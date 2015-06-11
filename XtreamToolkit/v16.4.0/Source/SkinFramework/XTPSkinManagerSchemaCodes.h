// XTPSkinManagerSchemaCodes.h
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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
#if !defined(__XTPSKINMANAGERSCHEMACODES_H__)
#define __XTPSKINMANAGERSCHEMACODES_H__
//}}AFX_CODEJOCK_PRIVATE

/*

The previous method of getting macro values from the header file
was smart and concise. Though hardly readable and not working for VS2012
(with __VSSYM32_H__ defined)

The issue is that SF code is using 'Tmschema.h' definitions, that came with WinXP
New theme api is defined in 'vssym32.h' which is not supported at the moment.

New approach is straightforward and portable, but verbose and not optimized.

Can be optimized in favor of speed using binary search.

*/

struct TMS_SCHEMA_ENTRY {
	TCHAR *str;
	int n;
};

static struct TMS_SCHEMA_ENTRY classAndPartsCodes[] =
{
	// CJ
	_T("Globals"),          1,
	_T("Documentation"),    2,
	_T("SysMetrics"),       3,
	_T("ListBox"),          4,
	_T("Dialog"),           5,

	// Tmsschema.h
	_T("WINDOW"),           6,
		_T("WINDOW_CAPTION"),           1,
		_T("WINDOW_SMALLCAPTION"),      2,
		_T("WINDOW_MINCAPTION"),        3,
		_T("WINDOW_SMALLMINCAPTION"),   4,
		_T("WINDOW_MAXCAPTION"),        5,
		_T("WINDOW_SMALLMAXCAPTION"),   6,
		_T("WINDOW_FRAMELEFT"),         7,
		_T("WINDOW_FRAMERIGHT"),        8,
		_T("WINDOW_FRAMEBOTTOM"),       9,
		_T("WINDOW_SMALLFRAMELEFT"),    10,
		_T("WINDOW_SMALLFRAMERIGHT"),   11,
		_T("WINDOW_SMALLFRAMEBOTTOM"),  12,
		//---- window frame buttons ----
		_T("WINDOW_SYSBUTTON"),         13,
		_T("WINDOW_MDISYSBUTTON"),      14,
		_T("WINDOW_MINBUTTON"),         15,
		_T("WINDOW_MDIMINBUTTON"),      16,
		_T("WINDOW_MAXBUTTON"),         17,
		_T("WINDOW_CLOSEBUTTON"),       18,
		_T("WINDOW_SMALLCLOSEBUTTON"),  19,
		_T("WINDOW_MDICLOSEBUTTON"),    20,
		_T("WINDOW_RESTOREBUTTON"),     21,
		_T("WINDOW_MDIRESTOREBUTTON"),  22,
		_T("WINDOW_HELPBUTTON"),        23,
		_T("WINDOW_MDIHELPBUTTON"),     24,
		//---- scrollbars
		_T("WINDOW_HORZSCROLL"),        25,
		_T("WINDOW_HORZTHUMB"),         26,
		_T("WINDOW_VERTSCROLL"),        27,
		_T("WINDOW_VERTTHUMB"),         28,
		//---- dialog ----
		_T("WINDOW_DIALOG"),            29,
		//---- hit-test templates ---
		_T("WINDOW_CAPTIONSIZINGTEMPLATE"),         30,
		_T("WINDOW_SMALLCAPTIONSIZINGTEMPLATE"),    31,
		_T("WINDOW_FRAMELEFTSIZINGTEMPLATE"),       32,
		_T("WINDOW_SMALLFRAMELEFTSIZINGTEMPLATE"),  33,
		_T("WINDOW_FRAMERIGHTSIZINGTEMPLATE"),      34,
		_T("WINDOW_SMALLFRAMERIGHTSIZINGTEMPLATE"), 35,
		_T("WINDOW_FRAMEBOTTOMSIZINGTEMPLATE"),     36,
		_T("WINDOW_SMALLFRAMEBOTTOMSIZINGTEMPLATE"),37,

	_T("BUTTON"),           7,
		_T("BUTTON_PUSHBUTTON"),        1,
		_T("BUTTON_RADIOBUTTON"),       2,
		_T("BUTTON_CHECKBOX"),          3,
		_T("BUTTON_GROUPBOX"),          4,
		_T("BUTTON_GROUPBOX"),          5,

	_T("REBAR"),            8,
		_T("REBAR_GRIPPER"),            1,
		_T("REBAR_GRIPPERVERT"),        2,
		_T("REBAR_BAND"),               3,
		_T("REBAR_CHEVRON"),            4,
		_T("REBAR_CHEVRONVERT"),        5,

	_T("TOOLBAR"),          9,
		_T("TOOLBAR_BUTTON"),               1,
		_T("TOOLBAR_DROPDOWNBUTTON"),       2,
		_T("TOOLBAR_SPLITBUTTON"),          3,
		_T("TOOLBAR_SPLITBUTTONDROPDOWN"),  4,
		_T("TOOLBAR_SEPARATOR"),            5,
		_T("TOOLBAR_SEPARATORVERT"),        6,

	_T("STATUS"),           10,
		_T("STATUS_PANE"),              1,
		_T("STATUS_GRIPPERPANE"),       2,
		_T("STATUS_GRIPPER"),           3,

	_T("MENU"),             11,
		_T("MENU_MENUITEM"),            1,
		_T("MENU_MENUDROPDOWN"),        2,
		_T("MENU_MENUBARITEM"),         3,
		_T("MENU_MENUBARDROPDOWN"),     4,
		_T("MENU_CHEVRON"),             5,
		_T("MENU_SEPARATOR"),           6,
		_T("MENU_BARBACKGROUND"),       7,
		_T("MENU_BARITEM"),             8,
		_T("MENU_POPUPBACKGROUND"),     9,
		_T("MENU_POPUPBORDERS"),        10,
		_T("MENU_POPUPCHECK"),          11,
		_T("MENU_POPUPCHECKBACKGROUND"),12,
		_T("MENU_POPUPGUTTER"),         13,
		_T("MENU_POPUPITEM"),           14,
		_T("MENU_POPUPSEPARATOR"),      15,
		_T("MENU_POPUPSUBMENU"),        16,
		_T("MENU_SYSTEMCLOSE"),         17,
		_T("MENU_SYSTEMMAXIMIZE"),      18,
		_T("MENU_SYSTEMMINIMIZE"),      19,
		_T("MENU_SYSTEMRESTORE"),       20,

	_T("LISTVIEW"),         12,
		_T("LISTVIEW_LISTITEM"),        1,
		_T("LISTVIEW_LISTGROUP"),       2,
		_T("LISTVIEW_LISTDETAIL"),      3,
		_T("LISTVIEW_LISTSORTEDDETAIL"),4,
		_T("LISTVIEW_EMPTYTEXT"),       5,

	_T("HEADER"),           13,
		_T("HEADER_HEADERITEM"),        1,
		_T("HEADER_HEADERITEMLEFT"),    2,
		_T("HEADER_HEADERITEMRIGHT"),   3,
		_T("HEADER_HEADERSORTARROW"),   4,

	_T("PROGRESS"),         14,
		_T("PROGRESS_BAR"),             1,
		_T("PROGRESS_BARVERT"),         2,
		_T("PROGRESS_CHUNK"),           3,
		_T("PROGRESS_CHUNKVERT"),       4,

	_T("TAB"),              15,
		_T("TAB_TABITEM"),              1,
		_T("TAB_TABITEMLEFTEDGE"),      2,
		_T("TAB_TABITEMRIGHTEDGE"),     3,
		_T("TAB_TABITEMBOTHEDGE"),      4,
		_T("TAB_TOPTABITEM"),           5,
		_T("TAB_TOPTABITEMLEFTEDGE"),   6,
		_T("TAB_TOPTABITEMRIGHTEDGE"),  7,
		_T("TAB_TOPTABITEMBOTHEDGE"),   8,
		_T("TAB_PANE"),                 9,
		_T("TAB_BODY"),                 10,

	_T("TRACKBAR"),         16,
		_T("TRACKBAR_TRACK"),           1,
		_T("TRACKBAR_TRACKVERT"),       2,
		_T("TRACKBAR_THUMB"),           3,
		_T("TRACKBAR_THUMBBOTTOM"),     4,
		_T("TRACKBAR_THUMBTOP"),        5,
		_T("TRACKBAR_THUMBVERT"),       6,
		_T("TRACKBAR_THUMBLEFT"),       7,
		_T("TRACKBAR_THUMBRIGHT"),      8,
		_T("TRACKBAR_TICS"),            9,
		_T("TRACKBAR_TICSVERT"),        10,

	_T("TOOLTIP"),          17,
		_T("TOOLTIP_STANDARD"),         1,
		_T("TOOLTIP_STANDARDTITLE"),    2,
		_T("TOOLTIP_BALLOON"),          3,
		_T("TOOLTIP_BALLOONTITLE"),     4,
		_T("TOOLTIP_CLOSE"),            5,

	_T("TREEVIEW"),         18,
		_T("TREEVIEW_TREEITEM"),        1,
		_T("TREEVIEW_GLYPH"),           2,
		_T("TREEVIEW_BRANCH"),          3,

	_T("SPIN"),             19,
		_T("SPIN_UP"),                  1,
		_T("SPIN_DOWN"),                2,
		_T("SPIN_UPHORZ"),              3,
		_T("SPIN_DOWNHORZ"),            4,

	_T("PAGE"),             20,
		_T("PAGE_UP"),                  1,
		_T("PAGE_DOWN"),                2,
		_T("PAGE_UPHORZ"),              3,
		_T("PAGE_DOWNHORZ"),            4,

	_T("SCROLLBAR"),        21,
		_T("SCROLLBAR_ARROWBTN"),       1,
		_T("SCROLLBAR_THUMBBTNHORZ"),   2,
		_T("SCROLLBAR_THUMBBTNVERT"),   3,
		_T("SCROLLBAR_LOWERTRACKHORZ"), 4,
		_T("SCROLLBAR_UPPERTRACKHORZ"), 5,
		_T("SCROLLBAR_LOWERTRACKVERT"), 6,
		_T("SCROLLBAR_UPPERTRACKVERT"), 7,
		_T("SCROLLBAR_GRIPPERHORZ"),    8,
		_T("SCROLLBAR_GRIPPERVERT"),    9,
		_T("SCROLLBAR_SIZEBOX"),        10,

	_T("EDIT"),             22,
		_T("EDIT_EDITTEXT"),            1,
		_T("EDIT_CARET"),               2,

	_T("COMBOBOX"),         23,
		_T("COMBOBOX_DROPDOWNBUTTON"),  1,

	_T("CLOCK"),            24,
		_T("CLOCK_TIME"),               1,

	_T("TRAYNOTIFY"),       25,
		_T("TRAYNOTIFY_BACKGROUND"),    1,
		_T("TRAYNOTIFY_ANIMBACKGROUND"),2,

	_T("TASKBAR"),          26,
		_T("TASKBAR_BACKGROUNDBOTTOM"),     1,
		_T("TASKBAR_BACKGROUNDRIGHT"),      2,
		_T("TASKBAR_BACKGROUNDTOP"),        3,
		_T("TASKBAR_BACKGROUNDLEFT"),       4,
		_T("TASKBAR_SIZINGBARBOTTOM"),      5,
		_T("TASKBAR_SIZINGBARRIGHT"),       6,
		_T("TASKBAR_SIZINGBARTOP"),         7,
		_T("TASKBAR_SIZINGBARLEFT"),        8,

	_T("TASKBAND"),         27,
		_T("TASKBAND_GROUPCOUNT"),              1,
		_T("TASKBAND_FLASHBUTTON"),             2,
		_T("TASKBAND_FLASHBUTTONGROUPMENU"),    3,

	_T("STARTPANEL"),       28,
		_T("STARTPANEL_USERPANE"),              1,
		_T("STARTPANEL_MOREPROGRAMS"),          2,
		_T("STARTPANEL_MOREPROGRAMSARROW"),     3,
		_T("STARTPANEL_PROGLIST"),              4,
		_T("STARTPANEL_PROGLISTSEPARATOR"),     5,
		_T("STARTPANEL_PLACESLIST"),            6,
		_T("STARTPANEL_PLACESLISTSEPARATOR"),   7,
		_T("STARTPANEL_LOGOFF"),                8,
		_T("STARTPANEL_LOGOFFBUTTONS"),         9,
		_T("STARTPANEL_USERPICTURE"),           10,
		_T("STARTPANEL_PREVIEW"),               11,

	_T("EXPLORERBAR"),      29,
		_T("EXPLORERBAR_HEADERBACKGROUND"),         1,
		_T("EXPLORERBAR_HEADERCLOSE"),              2,
		_T("EXPLORERBAR_HEADERPIN"),                3,
		_T("EXPLORERBAR_IEBARMENU"),                4,
		_T("EXPLORERBAR_NORMALGROUPBACKGROUND"),    5,
		_T("EXPLORERBAR_NORMALGROUPCOLLAPSE"),      6,
		_T("EXPLORERBAR_NORMALGROUPEXPAND"),        7,
		_T("EXPLORERBAR_NORMALGROUPHEAD"),          8,
		_T("EXPLORERBAR_SPECIALGROUPBACKGROUND"),   9,
		_T("EXPLORERBAR_SPECIALGROUPCOLLAPSE"),     10,
		_T("EXPLORERBAR_SPECIALGROUPEXPAND"),       11,
		_T("EXPLORERBAR_SPECIALGROUPHEAD"),         12,

	_T("MENUBAND"),         30,
		_T("MENUBAND_NEWAPPBUTTON"),            1,
		_T("MENUBAND_SEPERATOR"),               2,

	0,0
};

static struct TMS_SCHEMA_ENTRY enumCodes[] =
{
	_T("BGTYPE_IMAGEFILE"),         0,
	_T("BGTYPE_BORDERFILL"),        1,
	_T("BGTYPE_NONE"),              2,

	_T("IMAGELAYOUT_VERTICAL"),     0,
	_T("IMAGELAYOUT_HORIZONTAL"),   1,

	_T("BORDERTYPE_RECT"),          0,
	_T("BORDERTYPE_ROUNDRECT"),     1,
	_T("BORDERTYPE_ELLIPSE"),       2,

	_T("FILLTYPE_SOLID"),           0,
	_T("FILLTYPE_VERTGRADIENT"),    1,
	_T("FILLTYPE_HORZGRADIENT"),    2,
	_T("FILLTYPE_RADIALGRADIENT"),  3,
	_T("FILLTYPE_TILEIMAGE"),       4,

	_T("SIZINGTYPE_TRUESIZE"),      0,
	_T("SIZINGTYPE_STRETCH"),       1,
	_T("SIZINGTYPE_TILE"),          2,

	_T("HALIGN_LEFT"),              0,
	_T("HALIGN_CENTER"),            1,
	_T("HALIGN_RIGHT"),             2,

	_T("CONTENTALIGNMENT_LEFT"),    0,
	_T("CONTENTALIGNMENT_CENTER"),  1,
	_T("CONTENTALIGNMENT_RIGHT"),   2,

	_T("VALIGN_TOP"),               0,
	_T("VALIGN_CENTER"),            1,
	_T("VALIGN_BOTTOM"),            2,

	_T("OFFSETTYPE_TOPLEFT"),           0,
	_T("OFFSETTYPE_TOPRIGHT"),          1,
	_T("OFFSETTYPE_TOPMIDDLE"),         2,
	_T("OFFSETTYPE_BOTTOMLEFT"),        3,
	_T("OFFSETTYPE_BOTTOMRIGHT"),       4,
	_T("OFFSETTYPE_BOTTOMMIDDLE"),      5,
	_T("OFFSETTYPE_MIDDLELEFT"),        6,
	_T("OFFSETTYPE_MIDDLERIGHT"),       7,
	_T("OFFSETTYPE_LEFTOFCAPTION"),     8,
	_T("OFFSETTYPE_RIGHTOFCAPTION"),    9,
	_T("OFFSETTYPE_LEFTOFLASTBUTTON"),  10,
	_T("OFFSETTYPE_RIGHTOFLASTBUTTON"), 11,
	_T("OFFSETTYPE_ABOVELASTBUTTON"),   12,
	_T("OFFSETTYPE_BELOWLASTBUTTON"),   13,

	_T("ICONEFFECT_NONE"),          0,
	_T("ICONEFFECT_GLOW"),          1,
	_T("ICONEFFECT_SHADOW"),        2,
	_T("ICONEFFECT_PULSE"),         3,
	_T("ICONEFFECT_ALPHA"),         4,

	_T("TEXTSHADOWTYPE_NONE"),      0,
	_T("TEXTSHADOWTYPE_SINGLE"),    1,
	_T("TEXTSHADOWTYPE_CONTINUOUS"),2,

	_T("GLYPHTYPE_NONE"),           0,
	_T("GLYPHTYPE_IMAGEGLYPH"),     1,
	_T("GLYPHTYPE_FONTGLYPH"),      2,

	_T("IMAGESELECTTYPE_NONE"),     0,
	_T("IMAGESELECTTYPE_SIZE"),     1,
	_T("IMAGESELECTTYPE_DPI"),      2,

	_T("TRUESIZESCALINGTYPE_NONE"), 0,
	_T("TRUESIZESCALINGTYPE_SIZE"), 1,
	_T("TRUESIZESCALINGTYPE_DPI"),  2,

	_T("GLYPHFONTSIZINGTYPE_NONE"), 0,
	_T("GLYPHFONTSIZINGTYPE_SIZE"), 1,
	_T("GLYPHFONTSIZINGTYPE_DPI"),  2,

	0,0
};

static struct TMS_SCHEMA_ENTRY stateCodes[] =
{
	_T("FRAME_ACTIVE"),     1,
	_T("FRAME_INACTIVE"),   2,

	_T("CAPTION_ACTIVE"),   1,
	_T("CAPTION_INACTIVE"), 2,
	_T("CAPTION_DISABLED"), 3,

	_T("MAXCAPTION_ACTIVE"),    1,
	_T("MAXCAPTION_INACTIVE"),  2,
	_T("MAXCAPTION_DISABLED"),  3,

	_T("MINCAPTION_ACTIVE"),    1,
	_T("MINCAPTION_INACTIVE"),  2,
	_T("MINCAPTION_DISABLED"),  3,

	_T("HORZSCROLL_NORMAL"),    1,
	_T("HORZSCROLL_HOT"),       2,
	_T("HORZSCROLL_PUSHED"),    3,
	_T("HORZSCROLL_DISABLED"),  4,

	_T("HORZTHUMB_NORMAL"),     1,
	_T("HORZTHUMB_HOT"),        2,
	_T("HORZTHUMB_PUSHED"),     3,
	_T("HORZTHUMB_DISABLED"),   4,

	_T("VERTSCROLL_NORMAL"),    1,
	_T("VERTSCROLL_HOT"),       2,
	_T("VERTSCROLL_PUSHED"),    3,
	_T("VERTSCROLL_DISABLED"),  4,

	_T("VERTTHUMB_NORMAL"),     1,
	_T("VERTTHUMB_HOT"),        2,
	_T("VERTTHUMB_PUSHED"),     3,
	_T("VERTTHUMB_DISABLED"),   4,

	_T("SYSBUTTON_NORMAL"),     1,
	_T("SYSBUTTON_HOT"),        2,
	_T("SYSBUTTON_PUSHED"),     3,
	_T("SYSBUTTON_DISABLED"),   4,

	_T("MINBUTTON_NORMAL"),     1,
	_T("MINBUTTON_HOT"),        2,
	_T("MINBUTTON_PUSHED"),     3,
	_T("MINBUTTON_DISABLED"),   4,

	_T("MAXBUTTON_NORMAL"),     1,
	_T("MAXBUTTON_HOT"),        2,
	_T("MAXBUTTON_PUSHED"),     3,
	_T("MAXBUTTON_DISABLED"),   4,

	_T("RESTOREBUTTON_NORMAL"),     1,
	_T("RESTOREBUTTON_HOT"),        2,
	_T("RESTOREBUTTON_PUSHED"),     3,
	_T("RESTOREBUTTON_DISABLED"),   4,

	_T("HELPBUTTON_NORMAL"),    1,
	_T("HELPBUTTON_HOT"),       2,
	_T("HELPBUTTON_PUSHED"),    3,
	_T("HELPBUTTON_DISABLED"),  4,

	_T("CLOSEBUTTON_NORMAL"),   1,
	_T("CLOSEBUTTON_HOT"),      2,
	_T("CLOSEBUTTON_PUSHED"),   3,
	_T("CLOSEBUTTON_DISABLED"), 4,

	_T("PUSHBUTTON_NORMAL"),    1,
	_T("PUSHBUTTON_HOT"),       2,
	_T("PUSHBUTTON_PRESSED"),   3,
	_T("PUSHBUTTON_DISABLED"),  4,
	_T("PUSHBUTTON_DEFAULTED"), 5,

	_T("RADIOBUTTON_UNCHECKEDNORMAL"),  1,
	_T("RADIOBUTTON_UNCHECKEDHOT"),     2,
	_T("RADIOBUTTON_UNCHECKEDPRESSED"), 3,
	_T("RADIOBUTTON_UNCHECKEDDISABLED"),4,
	_T("RADIOBUTTON_CHECKEDNORMAL"),    5,
	_T("RADIOBUTTON_CHECKEDHOT"),       6,
	_T("RADIOBUTTON_CHECKEDPRESSED"),   7,
	_T("RADIOBUTTON_CHECKEDDISABLED"),  8,

	_T("CHECKBOX_UNCHECKEDNORMAL"),     1,
	_T("CHECKBOX_UNCHECKEDHOT"),        2,
	_T("CHECKBOX_UNCHECKEDPRESSED"),    3,
	_T("CHECKBOX_UNCHECKEDDISABLED"),   4,
	_T("CHECKBOX_CHECKEDNORMAL"),       5,
	_T("CHECKBOX_CHECKEDHOT"),          6,
	_T("CHECKBOX_CHECKEDPRESSED"),      7,
	_T("CHECKBOX_CHECKEDDISABLED"),     8,
	_T("CHECKBOX_MIXEDNORMAL"),         9,
	_T("CHECKBOX_MIXEDHOT"),            10,
	_T("CHECKBOX_MIXEDPRESSED"),        11,
	_T("CHECKBOX_MIXEDDISABLED"),       12,

	_T("GROUPBOX_NORMAL"),      1,
	_T("GROUPBOX_DISABLED"),    2,

	//---------------------------------------------------------------------------------------
	//   "Rebar" Parts & States
	//---------------------------------------------------------------------------------------

	_T("CHEVRON_NORMAL"),       1,
	_T("CHEVRON_HOT"),          2,
	_T("CHEVRON_PRESSED"),      3,

	//---------------------------------------------------------------------------------------
	//   "Toolbar" Parts & States
	//---------------------------------------------------------------------------------------

	_T("TOOLBAR_NORMAL"),       1,
	_T("TOOLBAR_HOT"),          2,
	_T("TOOLBAR_PRESSED"),      3,
	_T("TOOLBAR_DISABLED"),     4,
	_T("TOOLBAR_CHECKED"),      5,
	_T("TOOLBAR_HOTCHECKED"),   6,

	//---------------------------------------------------------------------------------------
	//   "Menu" Parts & States
	//---------------------------------------------------------------------------------------

	_T("MENU_NORMAL"),          1,
	_T("MENU_SELECTED"),        2,
	_T("MENU_DEMOTED"),         3,

	_T("BARBACKGROUND_ACTIVE"),         1,
	_T("BARBACKGROUND_INACTIVE"),       2,

	_T("POPUPCHECK_CHECKMARKNORMAL"),       1,
	_T("POPUPCHECK_CHECKMARKDISABLED"),     2,
	_T("POPUPCHECK_BULLETNORMAL"),          3,
	_T("POPUPCHECK_BULLETDISABLED"),        4,

	_T("POPUPCHECKBACKGROUND_DISABLED"),    1,
	_T("POPUPCHECKBACKGROUND_NORMAL"),      2,
	_T("POPUPCHECKBACKGROUND_BITMAP"),      3,

	_T("POPUPITEM_NORMAL"),         1,
	_T("POPUPITEM_HOT"),            2,
	_T("POPUPITEM_DISABLED"),       3,
	_T("POPUPITEM_DISABLEDHOT"),    4,

	_T("POPUPSUBMENU_NORMAL"),          1,
	_T("POPUPSUBMENU_DISABLED"),        2,

	//---------------------------------------------------------------------------------------
	//   "ListView" Parts & States
	//---------------------------------------------------------------------------------------

	_T("LISTITEM_NORMAL"),          1,
	_T("LISTITEM_HOT"),             2,
	_T("LISTITEM_SELECTED"),        3,
	_T("LISTITEM_DISABLED"),        4,
	_T("LISTITEM_SELECTEDNOTFOCUS"),5,

	//---------------------------------------------------------------------------------------
	//   "Header" Parts & States
	//---------------------------------------------------------------------------------------

	_T("HEADERITEM_NORMAL"),        1,
	_T("HEADERITEM_HOT"),           2,
	_T("HEADERITEM_PRESSED"),       3,

	_T("HEADERITEMLEFT_NORMAL"),        1,
	_T("HEADERITEMLEFT_HOT"),           2,
	_T("HEADERITEMLEFT_PRESSED"),       3,

	_T("HEADERITEMRIGHT_NORMAL"),       1,
	_T("HEADERITEMRIGHT_HOT"),          2,
	_T("HEADERITEMRIGHT_PRESSED"),      3,

	_T("HEADERSORTARROW_SORTEDUP"),     1,
	_T("HEADERSORTARROW_SORTEDDOWN"),   2,

	//---------------------------------------------------------------------------------------
	//   "Tab" Parts & States
	//---------------------------------------------------------------------------------------

	_T("TABITEM_NORMAL"),           1,
	_T("TABITEM_HOT"),              2,
	_T("TABITEM_SELECTED"),         3,
	_T("TABITEM_DISABLED"),         4,
	_T("TABITEM_FOCUSED"),          5,

	_T("TABITEMLEFTEDGE_NORMAL"),           1,
	_T("TABITEMLEFTEDGE_HOT"),              2,
	_T("TABITEMLEFTEDGE_SELECTED"),         3,
	_T("TABITEMLEFTEDGE_DISABLED"),         4,
	_T("TABITEMLEFTEDGE_FOCUSED"),          5,

	_T("TABITEMRIGHTEDGE_NORMAL"),          1,
	_T("TABITEMRIGHTEDGE_HOT"),             2,
	_T("TABITEMRIGHTEDGE_SELECTED"),        3,
	_T("TABITEMRIGHTEDGE_DISABLED"),        4,
	_T("TABITEMRIGHTEDGE_FOCUSED"),         5,

	_T("TABITEMBOTHEDGES_NORMAL"),          1,
	_T("TABITEMBOTHEDGES_HOT"),             2,
	_T("TABITEMBOTHEDGES_SELECTED"),        3,
	_T("TABITEMBOTHEDGES_DISABLED"),        4,
	_T("TABITEMBOTHEDGES_FOCUSED"),         5,

	_T("TOPTABITEM_NORMAL"),                1,
	_T("TOPTABITEM_HOT"),                   2,
	_T("TOPTABITEM_SELECTED"),              3,
	_T("TOPTABITEM_DISABLED"),              4,
	_T("TOPTABITEM_FOCUSED"),               5,

	_T("TOPTABITEMLEFTEDGE_NORMAL"),        1,
	_T("TOPTABITEMLEFTEDGE_HOT"),           2,
	_T("TOPTABITEMLEFTEDGE_SELECTED"),      3,
	_T("TOPTABITEMLEFTEDGE_DISABLED"),      4,
	_T("TOPTABITEMLEFTEDGE_FOCUSED"),       5,

	_T("TOPTABITEMRIGHTEDGE_NORMAL"),       1,
	_T("TOPTABITEMRIGHTEDGE_HOT"),          2,
	_T("TOPTABITEMRIGHTEDGE_SELECTED"),     3,
	_T("TOPTABITEMRIGHTEDGE_DISABLED"),     4,
	_T("TOPTABITEMRIGHTEDGE_FOCUSED"),      5,

	_T("TOPTABITEMBOTHEDGES_NORMAL"),       1,
	_T("TOPTABITEMBOTHEDGES_HOT"),          2,
	_T("TOPTABITEMBOTHEDGES_SELECTED"),     3,
	_T("TOPTABITEMBOTHEDGES_DISABLED"),     4,
	_T("TOPTABITEMBOTHEDGES_FOCUSED"),      5,

	//---------------------------------------------------------------------------------------
	//   "Trackbar" Parts & States
	//---------------------------------------------------------------------------------------

	_T("TRACKBAR_NORMAL"),          1,

	_T("TRACK_NORMAL"),             1,

	_T("TRACKVERT_NORMAL"),         1,

	_T("THUMB_NORMAL"),             1,
	_T("THUMB_HOT"),                2,
	_T("THUMB_PRESSED"),            3,
	_T("THUMB_FOCUSED"),            4,
	_T("THUMB_DISABLED"),           5,

	_T("THUMBBOTTOM_NORMAL"),       1,
	_T("THUMBBOTTOM_HOT"),          2,
	_T("THUMBBOTTOM_PRESSED"),      3,
	_T("THUMBBOTTOM_FOCUSED"),      4,
	_T("THUMBBOTTOM_DISABLED"),     5,

	_T("THUMBTOP_NORMAL"),          1,
	_T("THUMBTOP_HOT"),             2,
	_T("THUMBTOP_PRESSED"),         3,
	_T("THUMBTOP_FOCUSED"),         4,
	_T("THUMBTOP_DISABLED"),        5,

	_T("THUMBVERT_NORMAL"),         1,
	_T("THUMBVERT_HOT"),            2,
	_T("THUMBVERT_PRESSED"),        3,
	_T("THUMBVERT_FOCUSED"),        4,
	_T("THUMBVERT_DISABLED"),       5,

	_T("THUMBLEFT_NORMAL"),         1,
	_T("THUMBLEFT_HOT"),            2,
	_T("THUMBLEFT_PRESSED"),        3,
	_T("THUMBLEFT_FOCUSED"),        4,
	_T("THUMBLEFT_DISABLED"),       5,

	_T("THUMBRIGHT_NORMAL"),        1,
	_T("THUMBRIGHT_HOT"),           2,
	_T("THUMBRIGHT_PRESSED"),       3,
	_T("THUMBRIGHT_FOCUSED"),       4,
	_T("THUMBRIGHT_DISABLED"),      5,

	_T("TICS_NORMAL"),              1,

	_T("TICSVERT_NORMAL"),          1,

	//---------------------------------------------------------------------------------------
	//   "Tooltips" Parts & States
	//---------------------------------------------------------------------------------------

	_T("CLOSE_NORMAL"),         1,
	_T("CLOSE_HOT"),            2,
	_T("CLOSE_PRESSED"),        3,

	_T("STANDARD_NORMAL"),      1,
	_T("STANDARD_LINK"),        2,

	_T("BALLOON_NORMAL"),       1,
	_T("BALLOON_LINK"),         2,

	//---------------------------------------------------------------------------------------
	//   "TreeView" Parts & States
	//---------------------------------------------------------------------------------------

	_T("TREEITEM_NORMAL"),              1,
	_T("TREEITEM_HOT"),                 2,
	_T("TREEITEM_SELECTED"),            3,
	_T("TREEITEM_DISABLED"),            4,
	_T("TREEITEM_SELECTEDNOTFOCUS"),    5,

	_T("GLYPH_CLOSED"),                 1,
	_T("GLYPH_OPENED"),                 2,

	//---------------------------------------------------------------------------------------
	//   "Spin" Parts & States
	//---------------------------------------------------------------------------------------

	_T("UP_NORMAL"),            1,
	_T("UP_HOT"),               2,
	_T("UP_PRESSED"),           3,
	_T("UP_DISABLED"),          4,

	_T("DOWN_NORMAL"),          1,
	_T("DOWN_HOT"),             2,
	_T("DOWN_PRESSED"),         3,
	_T("DOWN_DISABLED"),        4,

	_T("UPHORZ_NORMAL"),        1,
	_T("UPHORZ_HOT"),           2,
	_T("UPHORZ_PRESSED"),       3,
	_T("UPHORZ_DISABLED"),      4,

	_T("DOWNHORZ_NORMAL"),      1,
	_T("DOWNHORZ_HOT"),         2,
	_T("DOWNHORZ_PRESSED"),     3,
	_T("DOWNHORZ_DISABLED"),    4,

	//---------------------------------------------------------------------------------------
	//   "Scrollbar" Parts & States
	//---------------------------------------------------------------------------------------

	_T("ARROWBTN_UPNORMAL"),        1,
	_T("ARROWBTN_UPHOT"),           2,
	_T("ARROWBTN_UPPRESSED"),       3,
	_T("ARROWBTN_UPDISABLED"),      4,
	_T("ARROWBTN_DOWNNORMAL"),      5,
	_T("ARROWBTN_DOWNHOT"),         6,
	_T("ARROWBTN_DOWNPRESSED"),     7,
	_T("ARROWBTN_DOWNDISABLED"),    8,
	_T("ARROWBTN_LEFTNORMAL"),      9,
	_T("ARROWBTN_LEFTHOT"),         10,
	_T("ARROWBTN_LEFTPRESSED"),     11,
	_T("ARROWBTN_LEFTDISABLED"),    12,
	_T("ARROWBTN_RIGHTNORMAL"),     13,
	_T("ARROWBTN_RIGHTHOT"),        14,
	_T("ARROWBTN_RIGHTPRESSED"),    15,
	_T("ARROWBTN_RIGHTDISABLED"),   16,

	_T("SCROLLBAR_NORMAL"),         1,
	_T("SCROLLBAR_HOT"),            2,
	_T("SCROLLBAR_PRESSED"),        3,
	_T("SCROLLBAR_DISABLED"),       4,
	_T("SCROLLBAR_ACTIVE"),         5,

	_T("SIZEBOX_RIGHTALIGN"),       1,
	_T("SIZEBOX_LEFTALIGN"),        2,

	//---------------------------------------------------------------------------------------
	//   "Edit" Parts & States
	//---------------------------------------------------------------------------------------

	_T("EDITTEXT_NORMAL"),          1,
	_T("EDITTEXT_HOT"),             2,
	_T("EDITTEXT_SELECTED"),        3,
	_T("EDITTEXT_DISABLED"),        4,
	_T("EDITTEXT_FOCUSED"),         5,
	_T("EDITTEXT_READONLY"),        6,
	_T("EDITTEXT_ASSIST"),          7,

	//---------------------------------------------------------------------------------------
	//   "ComboBox" Parts & States
	//---------------------------------------------------------------------------------------

	_T("COMBOBOX_NORMAL"),          1,
	_T("COMBOBOX_HOT"),             2,
	_T("COMBOBOX_PRESSED"),         3,
	_T("COMBOBOX_DISABLED"),        4,

	//---------------------------------------------------------------------------------------
	//   "Taskbar Clock" Parts & States
	//---------------------------------------------------------------------------------------

	_T("CLOCK_NORMAL"),         1,

	//---------------------------------------------------------------------------------------
	//   "StartPanel" Parts & States
	//---------------------------------------------------------------------------------------

	_T("MOREPROGRAMSARROW_NORMAL"),         1,
	_T("MOREPROGRAMSARROW_HOT"),            2,
	_T("MOREPROGRAMSARROW_PRESSED"),        3,

	_T("LOGOFFBUTTONS_NORMAL"),             1,
	_T("LOGOFFBUTTONS_HOT"),                2,
	_T("LOGOFFBUTTONS_PRESSED"),            3,

	//---------------------------------------------------------------------------------------
	//   "ExplorerBar" Parts & States
	//---------------------------------------------------------------------------------------

	_T("HEADERCLOSE_NORMAL"),           1,
	_T("HEADERCLOSE_HOT"),              2,
	_T("HEADERCLOSE_PRESSED"),          3,

	_T("HEADERPIN_NORMAL"),             1,
	_T("HEADERPIN_HOT"),                2,
	_T("HEADERPIN_PRESSED"),            3,
	_T("HEADERPIN_SELECTEDNORMAL"),     4,
	_T("HEADERPIN_SELECTEDHOT"),        5,
	_T("HEADERPIN_SELECTEDPRESSED"),    6,

	_T("IEBARMENU_NORMAL"),             1,
	_T("IEBARMENU_HOT"),                2,
	_T("IEBARMENU_PRESSED"),            3,

	_T("NORMALGROUPCOLLAPSE_NORMAL"),   1,
	_T("NORMALGROUPCOLLAPSE_HOT"),      2,
	_T("NORMALGROUPCOLLAPSE_PRESSED"),  3,

	_T("NORMALGROUPEXPAND_NORMAL"),     1,
	_T("NORMALGROUPEXPAND_HOT"),        2,
	_T("NORMALGROUPEXPAND_PRESSED"),    3,

	_T("SPECIALGROUPCOLLAPSE_NORMAL"),  1,
	_T("SPECIALGROUPCOLLAPSE_HOT"),     2,
	_T("SPECIALGROUPCOLLAPSE_PRESSED"), 3,

	_T("SPECIALGROUPEXPAND_NORMAL"),    1,
	_T("SPECIALGROUPEXPAND_HOT"),       2,
	_T("SPECIALGROUPEXPAND_PRESSED"),   3,

	//---------------------------------------------------------------------------------------
	//   "TaskBand" Parts & States
	//---------------------------------------------------------------------------------------

	_T("MENUBAND_NORMAL"),              1,
	_T("MENUBAND_HOT"),                 2,
	_T("MENUBAND_PRESSED"),             3,
	_T("MENUBAND_DISABLED"),            4,
	_T("MENUBAND_CHECKED"),             5,
	_T("MENUBAND_HOTCHECKED"),          6,

	0,0
};

//////////////////////////////////////////////////////////////////////////
// Property Codes. The actual number goes without the last digit.
// The last digit is a value from XTPSkinManagerProperty
//////////////////////////////////////////////////////////////////////////

#define PROPERY_N(n,t) (n*10+t)

#ifdef TM_PROP
#undef TM_PROP
#endif

#ifdef UNICODE
#define TM_PROP(val, prefix, name, primval) L"P_"##L#name, PROPERY_N(val, XTP_SKINPROPERTY_##primval),
#else
#define TM_PROP(val, prefix, name, primval)   "P_"###name, PROPERY_N(val, XTP_SKINPROPERTY_##primval),
#endif


static struct TMS_SCHEMA_ENTRY propertyCodes[] =
{
	// here goes blind copy from 'Tmschema.h'

	//---- primitive types ----
	TM_PROP(201, TMT, STRING,    STRING)
	TM_PROP(202, TMT, INT,       INT)
	TM_PROP(203, TMT, BOOL,      BOOL)
	TM_PROP(204, TMT, COLOR,     COLOR)
	TM_PROP(205, TMT, MARGINS,   MARGINS)
	TM_PROP(206, TMT, FILENAME,  FILENAME)
	TM_PROP(207, TMT, SIZE,      SIZE)
	TM_PROP(208, TMT, POSITION,  POSITION)
	TM_PROP(209, TMT, RECT,      RECT)
	TM_PROP(210, TMT, FONT,      FONT)
	TM_PROP(211, TMT, INTLIST,   INTLIST)

	//---- special misc. properties ----
	TM_PROP(401, TMT, COLORSCHEMES,   STRING)
	TM_PROP(402, TMT, SIZES,          STRING)
	TM_PROP(403, TMT, CHARSET,        INT)

	//---- [documentation] properties ----

	TM_PROP(601, TMT, DISPLAYNAME,    STRING)
	TM_PROP(602, TMT, TOOLTIP,        STRING)
	TM_PROP(603, TMT, COMPANY,        STRING)
	TM_PROP(604, TMT, AUTHOR,         STRING)
	TM_PROP(605, TMT, COPYRIGHT,      STRING)
	TM_PROP(606, TMT, URL,            STRING)
	TM_PROP(607, TMT, VERSION,        STRING)
	TM_PROP(608, TMT, DESCRIPTION,    STRING)

	//---- theme metrics: fonts ----

	TM_PROP(801, TMT, CAPTIONFONT,        FONT)
	TM_PROP(802, TMT, SMALLCAPTIONFONT,   FONT)
	TM_PROP(803, TMT, MENUFONT,           FONT)
	TM_PROP(804, TMT, STATUSFONT,         FONT)
	TM_PROP(805, TMT, MSGBOXFONT,         FONT)
	TM_PROP(806, TMT, ICONTITLEFONT,      FONT)

	//---- theme metrics: bools ----

	TM_PROP(1001, TMT, FLATMENUS,            BOOL)

	//---- theme metrics: sizes ----

	TM_PROP(1201, TMT, SIZINGBORDERWIDTH,    SIZE)
	TM_PROP(1202, TMT, SCROLLBARWIDTH,       SIZE)
	TM_PROP(1203, TMT, SCROLLBARHEIGHT,      SIZE)
	TM_PROP(1204, TMT, CAPTIONBARWIDTH,      SIZE)
	TM_PROP(1205, TMT, CAPTIONBARHEIGHT,     SIZE)
	TM_PROP(1206, TMT, SMCAPTIONBARWIDTH,    SIZE)
	TM_PROP(1207, TMT, SMCAPTIONBARHEIGHT,   SIZE)
	TM_PROP(1208, TMT, MENUBARWIDTH,         SIZE)
	TM_PROP(1209, TMT, MENUBARHEIGHT,        SIZE)

	//---- theme metrics: ints ----

	TM_PROP(1301, TMT, MINCOLORDEPTH,     INT)

	//---- theme metrics: strings ----

	TM_PROP(1401, TMT, CSSNAME,            STRING)
	TM_PROP(1402, TMT, XMLNAME,            STRING)

	//---- theme metrics: colors ----

	TM_PROP(1601, TMT, SCROLLBAR,          COLOR)
	TM_PROP(1602, TMT, BACKGROUND,         COLOR)
	TM_PROP(1603, TMT, ACTIVECAPTION,      COLOR)
	TM_PROP(1604, TMT, INACTIVECAPTION,    COLOR)
	TM_PROP(1605, TMT, MENU,               COLOR)
	TM_PROP(1606, TMT, WINDOW,             COLOR)
	TM_PROP(1607, TMT, WINDOWFRAME,        COLOR)
	TM_PROP(1608, TMT, MENUTEXT,           COLOR)
	TM_PROP(1609, TMT, WINDOWTEXT,         COLOR)
	TM_PROP(1610, TMT, CAPTIONTEXT,        COLOR)
	TM_PROP(1611, TMT, ACTIVEBORDER,       COLOR)
	TM_PROP(1612, TMT, INACTIVEBORDER,     COLOR)
	TM_PROP(1613, TMT, APPWORKSPACE,       COLOR)
	TM_PROP(1614, TMT, HIGHLIGHT,          COLOR)
	TM_PROP(1615, TMT, HIGHLIGHTTEXT,      COLOR)
	TM_PROP(1616, TMT, BTNFACE,            COLOR)
	TM_PROP(1617, TMT, BTNSHADOW,          COLOR)
	TM_PROP(1618, TMT, GRAYTEXT,           COLOR)
	TM_PROP(1619, TMT, BTNTEXT,            COLOR)
	TM_PROP(1620, TMT, INACTIVECAPTIONTEXT,     COLOR)
	TM_PROP(1621, TMT, BTNHIGHLIGHT,            COLOR)
	TM_PROP(1622, TMT, DKSHADOW3D,              COLOR)
	TM_PROP(1623, TMT, LIGHT3D,                 COLOR)
	TM_PROP(1624, TMT, INFOTEXT,                COLOR)
	TM_PROP(1625, TMT, INFOBK,                  COLOR)
	TM_PROP(1626, TMT, BUTTONALTERNATEFACE,     COLOR)
	TM_PROP(1627, TMT, HOTTRACKING,             COLOR)
	TM_PROP(1628, TMT, GRADIENTACTIVECAPTION,   COLOR)
	TM_PROP(1629, TMT, GRADIENTINACTIVECAPTION, COLOR)
	TM_PROP(1630, TMT, MENUHILIGHT,             COLOR)
	TM_PROP(1631, TMT, MENUBAR,                 COLOR)

	//---- hue substitutions ----
	TM_PROP(1801, TMT, FROMHUE1,  INT)
	TM_PROP(1802, TMT, FROMHUE2,  INT)
	TM_PROP(1803, TMT, FROMHUE3,  INT)
	TM_PROP(1804, TMT, FROMHUE4,  INT)
	TM_PROP(1805, TMT, FROMHUE5,  INT)
	TM_PROP(1806, TMT, TOHUE1,    INT)
	TM_PROP(1807, TMT, TOHUE2,    INT)
	TM_PROP(1808, TMT, TOHUE3,    INT)
	TM_PROP(1809, TMT, TOHUE4,    INT)
	TM_PROP(1810, TMT, TOHUE5,    INT)

	//---- color substitutions ----
	TM_PROP(2001, TMT, FROMCOLOR1,  COLOR)
	TM_PROP(2002, TMT, FROMCOLOR2,  COLOR)
	TM_PROP(2003, TMT, FROMCOLOR3,  COLOR)
	TM_PROP(2004, TMT, FROMCOLOR4,  COLOR)
	TM_PROP(2005, TMT, FROMCOLOR5,  COLOR)
	TM_PROP(2006, TMT, TOCOLOR1,    COLOR)
	TM_PROP(2007, TMT, TOCOLOR2,    COLOR)
	TM_PROP(2008, TMT, TOCOLOR3,    COLOR)
	TM_PROP(2009, TMT, TOCOLOR4,    COLOR)
	TM_PROP(2010, TMT, TOCOLOR5,    COLOR)

	//---- rendering BOOL properties ----
	TM_PROP(2201, TMT, TRANSPARENT,   BOOL)       // image has transparent areas (see TransparentColor)
	TM_PROP(2202, TMT, AUTOSIZE,      BOOL)       // if TRUE, nonclient caption width varies with text extent
	TM_PROP(2203, TMT, BORDERONLY,    BOOL)       // only draw the border area of the image
	TM_PROP(2204, TMT, COMPOSITED,    BOOL)       // control will handle the composite drawing
	TM_PROP(2205, TMT, BGFILL,        BOOL)       // if TRUE, TRUESIZE images should be drawn on bg fill
	TM_PROP(2206, TMT, GLYPHTRANSPARENT,  BOOL)   // glyph has transparent areas (see GlyphTransparentColor)
	TM_PROP(2207, TMT, GLYPHONLY,         BOOL)   // only draw glyph (not background)
	TM_PROP(2208, TMT, ALWAYSSHOWSIZINGBAR, BOOL)
	TM_PROP(2209, TMT, MIRRORIMAGE,         BOOL) // default=TRUE means image gets mirrored in RTL (Mirror) windows
	TM_PROP(2210, TMT, UNIFORMSIZING,       BOOL) // if TRUE, height & width must be uniformly sized
	TM_PROP(2211, TMT, INTEGRALSIZING,      BOOL) // for TRUESIZE and Border sizing; if TRUE, factor must be integer
	TM_PROP(2212, TMT, SOURCEGROW,          BOOL) // if TRUE, will scale up src image when needed
	TM_PROP(2213, TMT, SOURCESHRINK,        BOOL) // if TRUE, will scale down src image when needed

	//---- rendering INT properties ----
	TM_PROP(2401, TMT, IMAGECOUNT,        INT)    // the number of state images in an imagefile
	TM_PROP(2402, TMT, ALPHALEVEL,        INT)    // (0-255) alpha value for an icon (DrawThemeIcon part)
	TM_PROP(2403, TMT, BORDERSIZE,        INT)    // the size of the border line for bgtype=BorderFill
	TM_PROP(2404, TMT, ROUNDCORNERWIDTH,  INT)    // (0-100) % of roundness for rounded rects
	TM_PROP(2405, TMT, ROUNDCORNERHEIGHT, INT)    // (0-100) % of roundness for rounded rects
	TM_PROP(2406, TMT, GRADIENTRATIO1,    INT)    // (0-255) - amt of gradient color 1 to use (all must total=255)
	TM_PROP(2407, TMT, GRADIENTRATIO2,    INT)    // (0-255) - amt of gradient color 2 to use (all must total=255)
	TM_PROP(2408, TMT, GRADIENTRATIO3,    INT)    // (0-255) - amt of gradient color 3 to use (all must total = 255)
	TM_PROP(2409, TMT, GRADIENTRATIO4,    INT)    // (0-255) - amt of gradient color 4 to use (all must total=255)
	TM_PROP(2410, TMT, GRADIENTRATIO5,    INT)    // (0-255) - amt of gradient color 5 to use (all must total=255)
	TM_PROP(2411, TMT, PROGRESSCHUNKSIZE, INT)    // size of progress control chunks
	TM_PROP(2412, TMT, PROGRESSSPACESIZE, INT)    // size of progress control spaces
	TM_PROP(2413, TMT, SATURATION,        INT)    // (0-255) amt of saturation for DrawThemeIcon() part
	TM_PROP(2414, TMT, TEXTBORDERSIZE,    INT)    // size of border around text chars
	TM_PROP(2415, TMT, ALPHATHRESHOLD,    INT)    // (0-255) the min. alpha value of a pixel that is solid
	TM_PROP(2416, TMT, WIDTH,             SIZE)   // custom window prop: size of part (min. window)
	TM_PROP(2417, TMT, HEIGHT,            SIZE)   // custom window prop: size of part (min. window)
	TM_PROP(2418, TMT, GLYPHINDEX,        INT)    // for font-based glyphs, the char index into the font
	TM_PROP(2419, TMT, TRUESIZESTRETCHMARK, INT)  // stretch TrueSize image when target exceeds source by this percent
	TM_PROP(2420, TMT, MINDPI1,         INT)      // min DPI ImageFile1 was designed for
	TM_PROP(2421, TMT, MINDPI2,         INT)      // min DPI ImageFile1 was designed for
	TM_PROP(2422, TMT, MINDPI3,         INT)      // min DPI ImageFile1 was designed for
	TM_PROP(2423, TMT, MINDPI4,         INT)      // min DPI ImageFile1 was designed for
	TM_PROP(2424, TMT, MINDPI5,         INT)      // min DPI ImageFile1 was designed for

	//---- rendering FONT properties ----
	TM_PROP(2601, TMT, GLYPHFONT,         FONT)   // the font that the glyph is drawn with

	//---- rendering INTLIST properties ----
	// start with 2801
	// (from smallest to largest)
	//---- rendering FILENAME properties ----
	TM_PROP(3001, TMT, IMAGEFILE,         FILENAME)   // the filename of the image (or basename, for mult. images)
	TM_PROP(3002, TMT, IMAGEFILE1,        FILENAME)   // multiresolution image file
	TM_PROP(3003, TMT, IMAGEFILE2,        FILENAME)   // multiresolution image file
	TM_PROP(3004, TMT, IMAGEFILE3,        FILENAME)   // multiresolution image file
	TM_PROP(3005, TMT, IMAGEFILE4,        FILENAME)   // multiresolution image file
	TM_PROP(3006, TMT, IMAGEFILE5,        FILENAME)   // multiresolution image file
	TM_PROP(3007, TMT, STOCKIMAGEFILE,    FILENAME)   // These are the only images that you can call GetThemeBitmap on
	TM_PROP(3008, TMT, GLYPHIMAGEFILE,    FILENAME)   // the filename for the glyph image

	//---- rendering STRING properties ----
	TM_PROP(3201, TMT, TEXT,              STRING)

	//---- rendering POSITION (x and y values) properties ----
	TM_PROP(3401, TMT, OFFSET,            POSITION)   // for window part layout
	TM_PROP(3402, TMT, TEXTSHADOWOFFSET,  POSITION)   // where char shadows are drawn, relative to orig. chars
	TM_PROP(3403, TMT, MINSIZE,           POSITION)   // min dest rect than ImageFile was designed for
	TM_PROP(3404, TMT, MINSIZE1,          POSITION)   // min dest rect than ImageFile1 was designed for
	TM_PROP(3405, TMT, MINSIZE2,          POSITION)   // min dest rect than ImageFile2 was designed for
	TM_PROP(3406, TMT, MINSIZE3,          POSITION)   // min dest rect than ImageFile3 was designed for
	TM_PROP(3407, TMT, MINSIZE4,          POSITION)   // min dest rect than ImageFile4 was designed for
	TM_PROP(3408, TMT, MINSIZE5,          POSITION)   // min dest rect than ImageFile5 was designed for
	TM_PROP(3409, TMT, NORMALSIZE,        POSITION)   // size of dest rect that exactly source

	//---- rendering MARGIN properties ----
	TM_PROP(3601, TMT, SIZINGMARGINS,     MARGINS)    // margins used for 9-grid sizing
	TM_PROP(3602, TMT, CONTENTMARGINS,    MARGINS)    // margins that define where content can be placed
	TM_PROP(3603, TMT, CAPTIONMARGINS,    MARGINS)    // margins that define where caption text can be placed

	//---- rendering COLOR properties ----
	TM_PROP(3801, TMT, BORDERCOLOR,      COLOR)       // color of borders for BorderFill
	TM_PROP(3802, TMT, FILLCOLOR,        COLOR)       // color of bg fill
	TM_PROP(3803, TMT, TEXTCOLOR,        COLOR)       // color text is drawn in
	TM_PROP(3804, TMT, EDGELIGHTCOLOR,     COLOR)     // edge color
	TM_PROP(3805, TMT, EDGEHIGHLIGHTCOLOR, COLOR)     // edge color
	TM_PROP(3806, TMT, EDGESHADOWCOLOR,    COLOR)     // edge color
	TM_PROP(3807, TMT, EDGEDKSHADOWCOLOR,  COLOR)     // edge color
	TM_PROP(3808, TMT, EDGEFILLCOLOR,  COLOR)         // edge color
	TM_PROP(3809, TMT, TRANSPARENTCOLOR, COLOR)       // color of pixels that are treated as transparent (not drawn)
	TM_PROP(3810, TMT, GRADIENTCOLOR1,   COLOR)       // first color in gradient
	TM_PROP(3811, TMT, GRADIENTCOLOR2,   COLOR)       // second color in gradient
	TM_PROP(3812, TMT, GRADIENTCOLOR3,   COLOR)       // third color in gradient
	TM_PROP(3813, TMT, GRADIENTCOLOR4,   COLOR)       // forth color in gradient
	TM_PROP(3814, TMT, GRADIENTCOLOR5,   COLOR)       // fifth color in gradient
	TM_PROP(3815, TMT, SHADOWCOLOR,      COLOR)       // color of text shadow
	TM_PROP(3816, TMT, GLOWCOLOR,        COLOR)       // color of glow produced by DrawThemeIcon
	TM_PROP(3817, TMT, TEXTBORDERCOLOR,  COLOR)       // color of text border
	TM_PROP(3818, TMT, TEXTSHADOWCOLOR,  COLOR)       // color of text shadow
	TM_PROP(3819, TMT, GLYPHTEXTCOLOR,        COLOR)  // color that font-based glyph is drawn with
	TM_PROP(3820, TMT, GLYPHTRANSPARENTCOLOR, COLOR)  // color of transparent pixels in GlyphImageFile
	TM_PROP(3821, TMT, FILLCOLORHINT, COLOR)          // hint about fill color used (for custom controls)
	TM_PROP(3822, TMT, BORDERCOLORHINT, COLOR)        // hint about border color used (for custom controls)
	TM_PROP(3823, TMT, ACCENTCOLORHINT, COLOR)        // hint about accent color used (for custom controls)

	//---- rendering enum properties (must be declared in TM_ENUM section above) ----
	TM_PROP(4001, TMT, BGTYPE,           ENUM)        // basic drawing type for each part
	TM_PROP(4002, TMT, BORDERTYPE,       ENUM)        // type of border for BorderFill parts
	TM_PROP(4003, TMT, FILLTYPE,         ENUM)        // fill shape for BorderFill parts
	TM_PROP(4004, TMT, SIZINGTYPE,       ENUM)        // how to size ImageFile parts
	TM_PROP(4005, TMT, HALIGN,           ENUM)        // horizontal alignment for TRUESIZE parts & glyphs
	TM_PROP(4006, TMT, CONTENTALIGNMENT, ENUM)        // custom window prop: how text is aligned in caption
	TM_PROP(4007, TMT, VALIGN,           ENUM)        // horizontal alignment for TRUESIZE parts & glyphs
	TM_PROP(4008, TMT, OFFSETTYPE,       ENUM)        // how window part should be placed
	TM_PROP(4009, TMT, ICONEFFECT,       ENUM)        // type of effect to use with DrawThemeIcon
	TM_PROP(4010, TMT, TEXTSHADOWTYPE,   ENUM)        // type of shadow to draw with text
	TM_PROP(4011, TMT, IMAGELAYOUT,      ENUM)        // how multiple images are arranged (horz. or vert.)
	TM_PROP(4012, TMT, GLYPHTYPE,             ENUM)   // controls type of glyph in imagefile objects
	TM_PROP(4013, TMT, IMAGESELECTTYPE,       ENUM)   // controls when to select from IMAGEFILE1...IMAGEFILE5
	TM_PROP(4014, TMT, GLYPHFONTSIZINGTYPE,   ENUM)   // controls when to select a bigger/small glyph font size
	TM_PROP(4015, TMT, TRUESIZESCALINGTYPE,   ENUM)   // controls how TrueSize image is scaled

	//---- custom properties (used only by controls/shell) ----
	TM_PROP(5001, TMT, USERPICTURE,           BOOL)
	TM_PROP(5002, TMT, DEFAULTPANESIZE,       RECT)
	TM_PROP(5003, TMT, BLENDCOLOR,            COLOR)

	0,0
};

// restore old value of TM_PROP
#undef TM_PROP
#define TM_PROP(val, prefix, name, primval) {L#name, prefix##_##name, TMT_##primval},

#pragma warning(disable: 4996)

static int number_for_entry(TMS_SCHEMA_ENTRY *pArray, LPCTSTR key, LPCTSTR prefix = NULL)
{
	static CMap<CString, LPCTSTR, int, int> speedUpCache;

	int iCached = -1;

	TCHAR szHashName[255];
	ZeroMemory(szHashName, sizeof(szHashName));

	if (prefix)
		_tcscpy(szHashName, prefix);

	_tcscat(szHashName, key);

	if (!speedUpCache.Lookup(szHashName, iCached))
	{
		int i = 0;
		TCHAR *name = pArray[i].str;

		while (name)
		{
			if (_tcsicmp(name, szHashName) == 0)
			{
				speedUpCache[szHashName] = pArray[i].n;

				return pArray[i].n;
			}

			name = pArray[++i].str;
		}

		return -1;
	}

	return iCached;
}

static int number_for_entry2(TMS_SCHEMA_ENTRY *pArray, LPCTSTR key, LPCTSTR key2)
{
	TCHAR szContencanated[255];
	_tcscpy(szContencanated, key);
	_tcscat(szContencanated, _T("_"));
	_tcscat(szContencanated, key2);

	return number_for_entry(pArray, szContencanated);
}

#pragma warning(default: 4996)

#define GET_CLASS_CODE(cls)             number_for_entry(classAndPartsCodes, cls)
#define GET_ENUM_CODE(prop, val)        number_for_entry2(enumCodes, prop, val)
#define GET_CLASSPART_CODE(cls, part)   number_for_entry2(classAndPartsCodes, cls, part)
#define GET_STATE_CODE(part, state)     number_for_entry2(stateCodes, part, state)
#define GET_PROPERTY_CODE(prop)         number_for_entry(propertyCodes, prop, _T("P_"))

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPSKINMANAGERSCHEMACODES_H__
//}}AFX_CODEJOCK_PRIVATE
