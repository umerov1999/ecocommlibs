/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/
 
#ifndef extapicfg_h
#define extapicfg_h


#define EXTAPI extern
//#define INTERACTION_API_DLL_EXPORT
//#define PRINT_API_DLL_EXPORT
//#define DOCUMENT_API_DLL_EXPORT

#if defined(_WIN32)
    #define DYNAMIC_IMPORT extern _declspec(dllimport)
    #define DYNAMIC_EXPORT extern _declspec(dllexport)
    #define STATIC_IMPORT  extern
#elif defined(__SYMBIAN32__)
	#define DYNAMIC_IMPORT extern IMPORT_C
    #define DYNAMIC_EXPORT extern EXPORT_C
    #define STATIC_IMPORT  extern
#elif defined(__APPLE__)
    #define DYNAMIC_IMPORT
    #define DYNAMIC_EXPORT __attribute__((visibility("default")))
    #define STATIC_IMPORT  extern
#else 
    #define DYNAMIC_IMPORT extern 
    #define DYNAMIC_EXPORT extern 
    #define STATIC_IMPORT  extern
#endif

// DOCUMENT API
#if defined DOCUMENT_API_DLL_EXPORT
    // DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_DOCUMENT_H DYNAMIC_IMPORT
		#define EXTAPI_DOCUMENT_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_DOCUMENT_H DYNAMIC_EXPORT
		#define EXTAPI_DOCUMENT_C STATIC_IMPORT
	#endif
#elif defined DOCUMENT_API_DYNAMIC_IMPORT               
	// DLL import
    #define EXTAPI_DOCUMENT_H DYNAMIC_IMPORT
    #define EXTAPI_DOCUMENT_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_DOCUMENT_H STATIC_IMPORT
    #define EXTAPI_DOCUMENT_C STATIC_IMPORT
#endif

// INTERACTION API
#if defined INTERACTION_API_DLL_EXPORT
    // DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_INTERACTION_H DYNAMIC_IMPORT
		#define EXTAPI_INTERACTION_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_INTERACTION_H DYNAMIC_EXPORT
		#define EXTAPI_INTERACTION_C STATIC_IMPORT
	#endif
#elif defined INTERACTION_API_DYNAMIC_IMPORT
	// DLL import
    #define EXTAPI_INTERACTION_H DYNAMIC_IMPORT
    #define EXTAPI_INTERACTION_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_INTERACTION_H STATIC_IMPORT
    #define EXTAPI_INTERACTION_C STATIC_IMPORT
#endif

 // PRINT API
#if defined PRINT_API_DLL_EXPORT
    // DLL export
    #define EXTAPI_PRINT_H DYNAMIC_EXPORT
    #define EXTAPI_PRINT_C STATIC_IMPORT
#elif defined PRINT_API_DYNAMIC_IMPORT
	// DLL import
    #define EXTAPI_PRINT_H DYNAMIC_IMPORT
    #define EXTAPI_PRINT_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_PRINT_H STATIC_IMPORT
    #define EXTAPI_PRINT_C STATIC_IMPORT
#endif

	// LICENSE API
#if defined LICENSE_API_DLL_EXPORT
    // DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_LICENSE_H DYNAMIC_IMPORT
		#define EXTAPI_LICENSE_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_LICENSE_H DYNAMIC_EXPORT
		#define EXTAPI_LICENSE_C STATIC_IMPORT
	#endif
#elif defined LICENSE_API_DLL_IMPORT
	// DLL import
    #define EXTAPI_LICENSE_H DYNAMIC_IMPORT
    #define EXTAPI_LICENSE_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_LICENSE_H STATIC_IMPORT
    #define EXTAPI_LICENSE_C STATIC_IMPORT
#endif

	// COMMON API
#if defined COMMON_API_DLL_EXPORT
   // DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_COMMON_H DYNAMIC_IMPORT
		#define EXTAPI_COMMON_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_COMMON_H DYNAMIC_EXPORT
		#define EXTAPI_COMMON_C STATIC_IMPORT
	#endif
#elif defined COMMON_API_DLL_IMPORT
   // DLL import
    #define EXTAPI_COMMON_H DYNAMIC_IMPORT
    #define EXTAPI_COMMON_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_COMMON_H STATIC_IMPORT
    #define EXTAPI_COMMON_C STATIC_IMPORT
#endif

	// PORTABILITY API
#if defined PORTABILITY_API_DLL_EXPORT
   // DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_PORTABILITY_H DYNAMIC_IMPORT
		#define EXTAPI_PORTABILITY_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_PORTABILITY_H DYNAMIC_EXPORT
		#define EXTAPI_PORTABILITY_C STATIC_IMPORT
	#endif
#elif defined PORTABILITY_API_DLL_IMPORT
   // DLL import
    #define EXTAPI_PORTABILITY_H DYNAMIC_IMPORT
    #define EXTAPI_PORTABILITY_C STATIC_IMPORT
#else
    // Default.
    #define EXTAPI_PORTABILITY_H STATIC_IMPORT
    #define EXTAPI_PORTABILITY_C STATIC_IMPORT
#endif


// Render Module
#if defined RENDER_API_DLL_EXPORT
// DLL export
	#if defined(__SYMBIAN32__)
		#define EXTAPI_RENDER_H DYNAMIC_IMPORT
		#define EXTAPI_RENDER_C DYNAMIC_EXPORT
	#else
		#define EXTAPI_RENDER_H DYNAMIC_EXPORT
		#define EXTAPI_RENDER_C STATIC_IMPORT
	#endif
#elif defined RENDER_API_DLL_IMPORT
	// Dll import.
		#define EXTAPI_RENDER_H DYNAMIC_IMPORT
		#define EXTAPI_RENDER_C STATIC_IMPORT
#else
	// Default Use as source or static lib.
	#define EXTAPI_RENDER_H STATIC_IMPORT
	#define EXTAPI_RENDER_C STATIC_IMPORT
#endif


#endif // extapicfg_h
