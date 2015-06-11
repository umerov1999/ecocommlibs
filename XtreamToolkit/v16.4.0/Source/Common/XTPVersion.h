// XTPVersion.h Xtreme Toolkit version definitions file
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

#undef _XTPLIB_VERSION_
#undef _XTPLIB_VERSION_MAJOR
#undef _XTPLIB_VERSION_MINOR
#undef _XTPLIB_VERSION_REVISION
#undef _XTPLIB_VERSION_PREFIX
#undef _XTPLIB_VERSION_BUILD
#undef _XTP_PACKAGE_NAME

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit version definitions:
//////////////////////////////////////////////////////////////////////

// Xtreme Toolkit Pro version number
#define _XTPLIB_VERSION_                    MAKELONG(4, 16)

// Xtreme Toolkit Pro Major version number
#define _XTPLIB_VERSION_MAJOR               16

// Xtreme Toolkit Pro Minor version number
#define _XTPLIB_VERSION_MINOR               4

#define _XTPLIB_VERSION_REVISION            0

#define _XTPLIB_VERSION_BUILD               0

// Xtreme Toolkit Pro version prefix
#define _XTPLIB_VERSION_PREFIX              1640


//#define _XTP_DEMOMODE
#define _XTP_RETAIL

#define _XTP_PACKAGE_NAME_T _T("Xtreme Toolkit Pro")
#define _XTP_PACKAGE_NAME_W  L "Xtreme Toolkit Pro"
#define _XTP_PACKAGE_NAME_A    "Xtreme Toolkit Pro"
#define _XTP_PACKAGE_NAME   _XTP_PACKAGE_NAME_T

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit Package
//////////////////////////////////////////////////////////////////////
#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_CALENDAR)
#define _XTP_INCLUDE_CALENDAR
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_CHART)
#define _XTP_INCLUDE_CHART
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_COMMANDBARS)
#define _XTP_INCLUDE_COMMANDBARS
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_COMMON)
#define _XTP_INCLUDE_COMMON
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_CONTROLS)
#define _XTP_INCLUDE_CONTROLS
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_DOCKINGPANE)
#define _XTP_INCLUDE_DOCKINGPANE
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_FLOWGRAPH)
#define _XTP_INCLUDE_FLOWGRAPH
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_GRAPHICLIBRARY)
#define _XTP_INCLUDE_GRAPHICLIBRARY
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_PROPERTYGRID)
#define _XTP_INCLUDE_PROPERTYGRID
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_REPORTCONTROL)
#define _XTP_INCLUDE_REPORTCONTROL
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_RIBBON)
// Requires: TabManager, Controls, CommandBars
#define _XTP_INCLUDE_RIBBON
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_SHORTCUTBAR)
#define _XTP_INCLUDE_SHORTCUTBAR
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_SKINFRAMEWORK)
#define _XTP_INCLUDE_SKINFRAMEWORK
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_SYNTAXEDIT)
#define _XTP_INCLUDE_SYNTAXEDIT
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_TABMANAGER)
#define _XTP_INCLUDE_TABMANAGER
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_TASKPANEL)
#define _XTP_INCLUDE_TASKPANEL
#endif

#if !defined(_XTP_EXCLUDE_ALL) && !defined(_XTP_EXCLUDE_MARKUP)
#define _XTP_INCLUDE_MARKUP
#endif
