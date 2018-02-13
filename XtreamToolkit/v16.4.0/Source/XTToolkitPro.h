// XTToolkitPro.h
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

#pragma warning(disable : 4786) // Identifier was truncated to '255' characters in the debug information

#if (_MSC_VER > 1100)
#pragma warning(push)
#pragma pack(push, 8)
#endif

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>
#endif // _AFX_NO_OLE_SUPPORT

#ifdef _MFC_OVERRIDES_NEW
#define _INC_MALLOC
#endif

#include <afxpriv.h>    // MFC support for docking windows
#include <afxtempl.h>   // MFC template classes
#include <afxcview.h>   // MFC ListView / TreeView support
#include <afxmt.h>      // MFC Multithreaded Extensions (Syncronization Objects)


#ifndef AFX_STATIC
#define AFX_STATIC static
#endif

//------------------------------------------------------------------------
// NOTE: If using the Xtreme Toolkit Pro as a static library linked to an
// application that is dynamically linked with MFC, you will need to do the
// following:
//
// Open the XTPToolkit_Lib project workspace and select one of the
// Win32 Dynamic build settings and build the library. Add the following
// lines of code to your stdafx.h file:
//
// #define _XTP_STATICLINK
// #include <XTToolkitPro.h>
//
// Add the following line of code to your *.rc2 file after the comment:
// "Add manually edited resources here...":
//
// #include "XTToolkitPro.rc"
//------------------------------------------------------------------------

#undef _XTP_EXTRN

#if !defined(_AFXDLL) || defined(_XTP_STATICLINK)
#define _XTP_EXTRN
#else
#define _XTP_EXTRN  __declspec(dllimport)
#endif // !defined(_AFXDLL) || defined(_XTP_STATICLINK)

#undef _XTP_EXT_CLASS
#define _XTP_EXT_CLASS _XTP_EXTRN

#include "Common/XTPVersion.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma message("--------------------------------------------------------------------------------")
#pragma message("Including " _XTP_PACKAGE_NAME_A)

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_XTP_INCLUDE_COMMON)
	#include "Common/Includes.h"
#endif

#if defined(_XTP_INCLUDE_MARKUP)
	#pragma message("Including Markup         (Use _XTP_EXCLUDE_MARKUP         to exclude from build)")
	#include "Markup/Includes.h"
#endif

#if defined(_XTP_INCLUDE_TABMANAGER)
	#pragma message("Including TabManager     (Use _XTP_EXCLUDE_TABMANAGER     to exclude from build)")
	#include "TabManager/Includes.h"
#endif

#if defined(_XTP_INCLUDE_CONTROLS)
	#pragma message("Including Controls       (Use _XTP_EXCLUDE_CONTROLS       to exclude from build)")

	#if !defined(_XTP_INCLUDE_MARKUP)
		#pragma message("   - Markup controls excluded from build (requires Markup)")
	#endif

	#include "Controls/Includes.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_XTP_INCLUDE_CALENDAR)
	#pragma message("Including Calendar       (Use _XTP_EXCLUDE_CALENDAR       to exclude from build)")
	#include "Calendar/Includes.h"
#endif

#if defined(_XTP_INCLUDE_CHART)
	#pragma message("Including Chart          (Use _XTP_EXCLUDE_CHART          to exclude from build)")
	#include "Chart/Includes.h"
#endif

#if defined(_XTP_INCLUDE_COMMANDBARS)
	#pragma message("Including CommandBars    (Use _XTP_EXCLUDE_COMMANDBARS    to exclude from build)")

	#if !defined(_XTP_INCLUDE_CONTROLS)
		#pragma message("   - Customize sheet excluded from build (requires Controls)")
	#endif

	#if !defined(_XTP_INCLUDE_TABMANAGER)
		#pragma message("   - Tab toolbar     excluded from build (requires TabManager)")
		#pragma message("   - Tab client      excluded from build (requires TabManager)")
	#endif

	#include "CommandBars/Includes.h"
#endif

#if defined(_XTP_INCLUDE_DOCKINGPANE)
	#pragma message("Including DockingPane    (Use _XTP_EXCLUDE_DOCKINGPANE    to exclude from build)")
	#include "DockingPane/Includes.h"
#endif

#if defined(_XTP_INCLUDE_FLOWGRAPH)
	#pragma message("Including FlowGraph      (Use _XTP_EXCLUDE_FLOWGRAPH      to exclude from build)")
	#include "FlowGraph/Includes.h"
#endif

#if defined(_XTP_INCLUDE_GRAPHICLIBRARY)
	#pragma message("Including GraphicLibrary (Use _XTP_EXCLUDE_GRAPHICLIBRARY to exclude from build)")
	#include "GraphicLibrary/Includes.h"
#endif

#if defined(_XTP_INCLUDE_PROPERTYGRID)
	#pragma message("Including PropertyGrid   (Use _XTP_EXCLUDE_PROPERTYGRID   to exclude from build)")
	#include "PropertyGrid/Includes.h"
#endif

#if defined(_XTP_INCLUDE_REPORTCONTROL)
	#pragma message("Including ReportControl  (Use _XTP_EXCLUDE_REPORTCONTROL  to exclude from build)")
	#include "ReportControl/Includes.h"
#endif

#if defined(_XTP_INCLUDE_RIBBON)
	#pragma message("Including Ribbon         (Use _XTP_EXCLUDE_RIBBON         to exclude from build)")

	#if !defined(_XTP_INCLUDE_TABMANAGER)
		#pragma message("   - Ribbon          excluded from build (requires TabManager)")
		#undef _XTP_INCLUDE_RIBBON
	#endif

	#if !defined(_XTP_INCLUDE_CONTROLS)
		#pragma message("   - Ribbon          excluded from build (requires Controls)")
		#undef _XTP_INCLUDE_RIBBON
	#endif

	#if !defined(_XTP_INCLUDE_COMMANDBARS)
		#pragma message("   - Ribbon          excluded from build (requires CommandBars)")
		#undef _XTP_INCLUDE_RIBBON
	#endif

	#if defined(_XTP_INCLUDE_TABMANAGER) && \
	   defined(_XTP_INCLUDE_CONTROLS)   && \
	   defined(_XTP_INCLUDE_COMMANDBARS)
		#include "Ribbon/Includes.h"
	#endif
#endif

#if defined(_XTP_INCLUDE_SHORTCUTBAR)
	#pragma message("Including ShortcutBar    (Use _XTP_EXCLUDE_SHORTCUTBAR    to exclude from build)")
	#include "ShortcutBar/Includes.h"
#endif

#if defined(_XTP_INCLUDE_SKINFRAMEWORK)
	#pragma message("Including SkinFramework  (Use _XTP_EXCLUDE_SKINFRAMEWORK  to exclude from build)")
	#include "SkinFramework/Includes.h"
#endif

#if defined(_XTP_INCLUDE_SYNTAXEDIT)
	#pragma message("Including SyntaxEdit     (Use _XTP_EXCLUDE_SYNTAXEDIT     to exclude from build)")
	#include "SyntaxEdit/Includes.h"
#endif

#if defined(_XTP_INCLUDE_TASKPANEL)
	#pragma message("Including TaskPanel      (Use _XTP_EXCLUDE_TASKPANEL      to exclude from build)")
	#include "TaskPanel/Includes.h"
#endif

#pragma message("--------------------------------------------------------------------------------")

///////////////////////////////////////////////////////////////////////////////////////////////////

#define _XTPLIB_FILE_VERSION_STRING_(x) #x
#define _XTPLIB_FILE_VERSION_STRING(x) _XTPLIB_FILE_VERSION_STRING_(x)

#if !defined(_XTP_DEMOMODE) && !defined(_XTP_RETAIL)
#define _XTPLIB_VISUALSTUDIO_VERSION ""
#else
#if _MSC_VER < 1200
#define _XTPLIB_VISUALSTUDIO_VERSION "vc50"
#endif
#if _MSC_VER == 1200
#define _XTPLIB_VISUALSTUDIO_VERSION "vc60"
#endif
#if _MSC_VER == 1300
#define _XTPLIB_VISUALSTUDIO_VERSION "vc70"
#endif
#if _MSC_VER == 1310
#define _XTPLIB_VISUALSTUDIO_VERSION "vc71"
#endif
#if _MSC_VER == 1400
#define _XTPLIB_VISUALSTUDIO_VERSION "vc80"
#endif
#if _MSC_VER == 1500
#define _XTPLIB_VISUALSTUDIO_VERSION "vc90"
#endif
#if _MSC_VER == 1600
#define _XTPLIB_VISUALSTUDIO_VERSION "vc100"
#endif

#ifndef _XTPLIB_VISUALSTUDIO_VERSION
#error "Unknown Visual Studio version"
#endif
#endif

#define _XTPLIB_FILE_VERSION _XTPLIB_FILE_VERSION_STRING(_XTPLIB_VERSION_PREFIX)


//------------------------------------------------------
// Xtreme link to the appropriate dll or static library:
//------------------------------------------------------
#if defined(_AFXDLL) && defined(_XTP_STATICLINK)
	#if defined(_DEBUG)
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "DSUD"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION "DSD"
		#endif // _UNICODE
	#else
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "DSU"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION "DS"
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "lib"
	#define _XTPLIB_LINK_IS_DLL 0
#elif !defined(_AFXDLL)
	#if defined(_DEBUG)
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "SUD"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION "SD"
		#endif // _UNICODE
	#else
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "SU"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION "S"
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "lib"
	#define _XTPLIB_LINK_IS_DLL 0
#else
	#if defined(_DEBUG)
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "UD"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION "D"
		#endif // _UNICODE
	#else
		#if defined(_UNICODE)
			#define _XTPLIB_PROJECT_CONFIGURATION "U"
		#else
			#define _XTPLIB_PROJECT_CONFIGURATION ""
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "dll"
	#define _XTPLIB_LINK_IS_DLL 1
#endif // !defined(_AFXDLL) || defined(_XTP_STATICLINK)

#if (_XTPLIB_LINK_IS_DLL == 0) && defined(_XTP_DEMOMODE)
	#pragma message(" ")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" The evaluation version of the toolkit only supports DLL configurations.")
	#pragma message(" To purchase the full version (with static link support) please visit http://www.codejock.com")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" ")
	#error This build configuration is not supported by the evaluation library
#endif

#if !(defined(_DEBUG)) && defined(_XTP_DEMOMODE)
	#pragma message(" ")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" The evaluation version of the toolkit only supports DEBUG configurations.")
	#pragma message(" To purchase the full version (with static link support) please visit http://www.codejock.com")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" ")
	#error This build configuration is not supported by the evaluation library
#endif


#if defined(_XTP_DEMOMODE)
	#define _XTPLIB_PROJECT_EVAL "Eval"
#else
	#define _XTPLIB_PROJECT_EVAL ""
#endif

#if defined(_WIN64)
	#define _XTPLIB_PLATFORM "x64"
#else
	#define _XTPLIB_PLATFORM ""
#endif

#ifndef _XTPLIB_NOAUTOLINK
	#pragma comment(lib, "ToolkitPro" _XTPLIB_PROJECT_EVAL _XTPLIB_FILE_VERSION _XTPLIB_VISUALSTUDIO_VERSION _XTPLIB_PLATFORM _XTPLIB_PROJECT_CONFIGURATION ".lib")
	#pragma message("Automatically linking with ToolkitPro" _XTPLIB_PROJECT_EVAL _XTPLIB_FILE_VERSION _XTPLIB_VISUALSTUDIO_VERSION _XTPLIB_PLATFORM _XTPLIB_PROJECT_CONFIGURATION "." _XTPLIB_LINK_TYPE)
#endif //_XTPLIB_NOAUTOLINK

#if !defined(_AFXDLL) && !defined(_XTPLIB_NOAUTOLIBIGNORE)
#ifdef _DEBUG
	#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
#else
	#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#endif
#endif

#undef _XTPLIB_FILE_VERSION
#undef _XTPLIB_PROJECT_CONFIGURATION
#undef _XTPLIB_PROJECT_EVAL
#undef _XTPLIB_LINK_TYPE
#undef _XTPLIB_LINK_IS_DLL

#if (_MSC_VER > 1100)
#pragma pack(pop)
#pragma warning(pop)
#endif

/////////////////////////////////////////////////////////////////////////////
