// Deprecated.h : header file
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
#if !defined(__XTPCONTROLS_DEPRECATED_H__)
#define __XTPCONTROLS_DEPRECATED_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// enumerators
// ----------------------

typedef XTPControlTheme XTPButtonTheme;
const XTPButtonTheme xtpButtonThemeStandard   = xtpControlThemeDefault;
const XTPButtonTheme xtpButtonThemeFlat       = xtpControlThemeFlat;
const XTPButtonTheme xtpButtonThemeUltraFlat  = xtpControlThemeUltraFlat;
const XTPButtonTheme xtpButtonThemeOffice2000 = xtpControlThemeOffice2000;
const XTPButtonTheme xtpButtonThemeOfficeXP   = xtpControlThemeOfficeXP;
const XTPButtonTheme xtpButtonThemeOffice2003 = xtpControlThemeOffice2003;
const XTPButtonTheme xtpButtonThemeOffice2007 = xtpControlThemeResource;

typedef XTPAnimationDCType XTAnimationType;
const XTAnimationType xtWindowsDefault = xtpAniDefault;
const XTAnimationType xtRandom         = xtpAniRandom;
const XTAnimationType xtUnfold         = xtpAniUnfold;
const XTAnimationType xtSlide          = xtpAniSlide;
const XTAnimationType xtFade           = xtpAniFade;
const XTAnimationType xtNone           = xtpAniNone;

typedef XTPResize XTResize;
const XTResize xtResizeNoSizeIcon         = xtpResizeNoSizeIcon;
const XTResize xtResizeNoHorizontal       = xtpResizeNoHorizontal;
const XTResize xtResizeNoVertical         = xtpResizeNoVertical;
const XTResize xtResizeNoMinsize          = xtpResizeNoMinsize;
const XTResize xtResizeNoClipChildren     = xtpResizeNoClipChildren;
const XTResize xtResizeNoTransparentGroup = xtpResizeNoTransparentGroup;

const XTPControlTheme xtpControlThemeOffice2007 = xtpControlThemeResource;

typedef XTPArrowIcon XTArrowIcon;
const XTArrowIcon xtArrowIconLeft      = xtpArrowIconLeft;
const XTArrowIcon xtArrowIconLeftHome  = xtpArrowIconLeftHome;
const XTArrowIcon xtArrowIconRight     = xtpArrowIconRight;
const XTArrowIcon xtArrowIconRightHome = xtpArrowIconRightHome;

typedef XTPFontStyle XTFontStyle;
const XTFontStyle xtFontUnknown   = xtpFontUnknown;
const XTFontStyle xtFontGUI       = xtpFontGUI;
const XTFontStyle xtFontSample    = xtpFontSample;
const XTFontStyle xtFontBoth      = xtpFontBoth;

typedef XTPListBoxStyle XTListBoxStyle;
const XTListBoxStyle xtListBoxStandard   = xtpListBoxStandard;
const XTListBoxStyle xtListBoxOfficeXP   = xtpListBoxOfficeXP;
const XTListBoxStyle xtListBoxOffice2007 = xtpListBoxOffice2007;

typedef XTPFindType XTFindType;
const XTFindType xtFindTypeDrive  = xtpFindTypeDrive;
const XTFindType xtFindTypeFolder = xtpFindTypeFolder;
const XTFindType xtFindTypeRoot   = xtpFindTypeRoot;
const XTFindType xtFindTypeShare  = xtpFindTypeShare;

typedef XTPSortType XTSortType;
const XTPSortType xtSortInt      = xtpSortInt;
const XTPSortType xtSortString   = xtpSortString;
const XTPSortType xtSortDateTime = xtpSortDateTime;
const XTPSortType xtSortDecimal  = xtpSortDecimal;

typedef XTPNavBtnState XTNavBtnState;
const XTNavBtnState xtNavBtnArrows  = xtpNavBtnArrows;
const XTNavBtnState xtNavBtnClose   = xtpNavBtnClose;
const XTNavBtnState xtNavBtnShowAll = xtpNavBtnShowAll;

// types
// ----------------

#define XT_RESIZE XTP_RESIZE

// structs
// ------------------

#define XT_RESIZERECT   XTP_RESIZERECT
#define XT_RESIZEPOINT  XTP_RESIZEPOINT
#define XT_LVITEMDATA   XTP_LVITEMDATA
#define XT_TVITEMDATA   XTP_TVITEMDATA

// classes
// ------------------

#define CXTAnimationMemDC                   CXTPAnimationDC
#define CXTBrowseDialog                     CXTPBrowseDialog
#define CXTBrowseButton                     CXTPBrowseButton
#define CXTEdit                             CXTPEdit
#define CXTBrowseEdit                       CXTPBrowseEdit
#define CXTItemEdit                         CXTPItemEdit
#define CXTCaptionButton                    CXTPCaptionButton
#define CXTCaption                          CXTPCaption
#define CXTCaptionPopupWnd                  CXTPCaptionPopupWnd
#define CXTCaptionTheme                     CXTPCaptionTheme
#define CXTCaptionThemeOfficeXP             CXTPCaptionThemeOfficeXP
#define CXTCaptionThemeOffice2003           CXTPCaptionThemeOffice2003
#define CXTCaptionButtonTheme               CXTPCaptionButtonTheme
#define CXTCaptionButtonThemeOfficeXP       CXTPCaptionButtonThemeOfficeXP
#define CXTCaptionButtonThemeOffice2003     CXTPCaptionButtonThemeOffice2003
#define CXTCheckListBox                     CXTPCheckListBox
#define CXTEyeDropper                       CXTPEyeDropper
#define CXTColorDialog                      CXTPColorDialog
#define CXTColorBase                        CXTPColorBase
#define CXTColorWnd                         CXTPColorWnd
#define CXTColorLum                         CXTPColorLum
#define CXTColorPageCustom                  CXTPColorPageCustom
#define CXTColorHex                         CXTPColorHex
#define CXTColorPageStandard                CXTPColorPageStandard
#define CXTColorPicker                      CXTPColorPicker
#define CXTColorPopup                       CXTPColorPopup
#define CXTColorSelectorCtrl                CXTPColorSelectorCtrl
#define CXTColorSelectorCtrlTheme           CXTPColorSelectorCtrlTheme
#define CXTColorSelectorCtrlThemeOfficeXP   CXTPColorSelectorCtrlThemeOfficeXP
#define CXTColorSelectorCtrlThemeOffice2003 CXTPColorSelectorCtrlThemeOffice2003
#define CXTColorSelectorCtrlThemeOffice2007 CXTPColorSelectorCtrlThemeResource
#define CXTDirWatcher                       CXTPDirWatcher
#define CXTDropSource                       CXTPDropSource
#define CXTEditListBoxToolBar               CXTPEditListBoxToolBar
#define CXTEditListBox                      CXTPEditListBox
#define CXTExcelTabCtrlButtonState          CXTPExcelTabCtrlButtonState
#define CXTExcelTabCtrl                     CXTPExcelTabCtrl
#define CXTFlatTabCtrl                      CXTPExcelTabCtrl
#define CXTExcelTabCtrlTheme                CXTPExcelTabCtrlTheme
#define CXTExcelTabCtrlThemeOfficeXP        CXTPExcelTabCtrlThemeOfficeXP
#define CXTExcelTabCtrlThemeOffice2003      CXTPExcelTabCtrlThemeOffice2003
#define CXTFontList                         CXTPFontList
#define CXTFontEnum                         CXTPFontEnum
#define CXTFontListBox                      CXTPFontListBox
#define CXTFontCombo                        CXTPFontComboBox
#define CXTTcbItem                          CXTPTcbItem
#define CXTNonClientMetrics                 CXTPNonClientMetrics
#define CXTLogFont                          CXTPLogFont
#define CXTAuxData                          CXTPAuxData
#define CXTHeaderCtrl                       CXTPHeaderCtrl
#define CXTHeaderCtrlTheme                  CXTPHeaderCtrlTheme
#define CXTHeaderCtrlThemeOfficeXP          CXTPHeaderCtrlThemeOfficeXP
#define CXTHeaderCtrlThemeOffice2003        CXTPHeaderCtrlThemeOffice2003
#define CXTHeaderCtrlThemeExplorer          CXTPHeaderCtrlThemeExplorer
#define CXTContextObjectHandler             CXTPContextObjectHandler
#define CXTContextBkModeHandler             CXTPContextBkModeHandler
#define CXTContextTextColorHandler          CXTPContextTextColorHandler
#define CXTContextBkColorHandler            CXTPContextBkColorHandler
#define CXTContextViewPortOrgHandler        CXTPContextViewPortOrgHandler
#define CXTHexEditBase                      CXTPHexEditBase
#define CXTHexEdit                          CXTPHexEdit
#define CXTHyperLink                        CXTPHyperLink
#define CXTListBase                         CXTPListBase
#define CXTListBox                          CXTPListBox
#define CXTListView                         CXTPListView
#define CXTListCtrl                         CXTPListCtrl
#define CXTMaskEdit                         CXTPMaskEdit
#define CXTDateEdit                         CXTPDateEdit
#define CXTTimeEdit                         CXTPTimeEdit
#define CXTMDIClientWnd                     CXTPMDIClientWnd
#define CXTMDIWndTab                        CXTPMDIWndTab
#define CXTMemFile                          CXTPMemFile
#define CXTMenuListBox                      CXTPShortcutListBox
#define CXTMenuListBoxTheme                 CXTPShortcutListBoxTheme
#define CXTMenuListBoxThemeOfficeXP         CXTPShortcutListBoxThemeOfficeXP
#define CXTMenuListBoxThemeOffice2003       CXTPShortcutListBoxThemeOffice2003
#define CXTPagerCtrl                        CXTPPageScroller
#define CXTRegistryManager                  CXTPRegistryManager
#define CXTResizeItem                       CXTPResizeItem
#define CXTSizeIcon                         CXTPSizeIcon
#define CXTResize                           CXTPResize
#define CXTResizeDialog                     CXTPResizeDialog
#define CXTResizePropertySheet              CXTPResizePropertySheet
#define CXTResizePropertyPage               CXTPResizePropertyPage
#define CXTResizeFormView                   CXTPResizeFormView
#define CXTResizeGroupBox                   CXTPResizeGroupBox
#define CXTResizeRect                       CXTPResizeRect
#define CXTResizePoint                      CXTPResizePoint
#define CXTSearchOptionsCtrl                CXTPSearchOptionsCtrl
#define CXTSearchOptionsView                CXTPSearchOptionsView
#define CXTShellListBase                    CXTPShellListBase
#define CXTShellListView                    CXTPShellListView
#define CXTShellListCtrl                    CXTPShellListCtrl
#define CXTShellListCtrlEx                  CXTPShellListCtrlEx
#define CXTShellPidl                        CXTPShellPidl
#define CXTShellSettings                    CXTPShellSettings
#define CXTShellTreeBase                    CXTPShellTreeBase
#define CXTShellTreeView                    CXTPShellTreeView
#define CXTShellTreeCtrl                    CXTPShellTreeCtrl
#define CXTSortClass                        CXTPSortClass
#define CXTSplitterWnd                      CXTPSplitterWnd
#define CXTSplitterWndEx                    CXTPSplitterWndEx
#define CXTSplitterWndTheme                 CXTPSplitterWndTheme
#define CXTSplitterWndThemeOfficeXP         CXTPSplitterWndThemeOfficeXP
#define CXTSplitterWndThemeOffice2003       CXTPSplitterWndThemeOffice2003
#define CXTTabBase                          CXTPTabBase
#define CXTTabExBase                        CXTPTabExBase
#define CXTTabCtrlBaseEx                    CXTPTabCtrlBaseEx
#define CXTTabBaseTheme                     CXTPTabBaseTheme
#define CXTTabBaseThemeOfficeXP             CXTPTabBaseThemeOfficeXP
#define CXTTabBaseThemeOffice2003           CXTPTabBaseThemeOffice2003
#define CXTTabCtrl                          CXTPTabCtrl
#define CXTTabCtrlButton                    CXTPTabCtrlButton
#define CXTTabCtrlButtons                   CXTPTabCtrlButtons
#define CXTCtrlView                         CXTPCtrlView
#define CXTTabView                          CXTPTabView
#define CXTTipOfTheDay                      CXTPTipOfTheDay
#define CXTTipWindow                        CXTPTipWindow
#define CXTTrayIcon                         CXTPTrayIcon
#define CXTTreeBase                         CXTPTreeBase
#define CXTTreeView                         CXTPTreeView
#define CXTTreeCtrl                         CXTPTreeCtrl
#define CXTIconHandle                       CXTPIconHandle
#define CXTNoFlickerWnd                     CXTPNoFlickerWnd
#define CXTSplitPath                        CXTPSplitPath
#define CXTWindowList                       CXTPWindowList
#define CXTWindowPos                        CXTPWindowPos
#define CXTFlatHeaderCtrl                   CXTPHeaderCtrl

// functions
// ---------

#define XTFuncInitDLL              XTPInitDLL
#define XTFuncChangeWindowFont     XTPChangeWindowFont
#define XTFuncDrawEmbossed         XTPDrawEmbossed
#define XTFuncDrawShadedRect       XTPDrawShadedRect
#define XTFuncChildWindowFromPoint XTPChildWindowFromPoint
#define XTFuncPathExists           XTPPathExists
#define XTFuncCreateView           XTPCreateView
#define XTFuncContextMenu          XTPContextMenu
#define XTAuxData                  XTPAuxData

// DDX
// ------------------

#define DDX_XTColorPicker DDX_XTPColorPicker
#define DDX_XTOleDateTime DDX_XTPOleDateTime

// defines
// ------------------

#define SZ_RESIZE              XTP_ATTR_RESIZE
#define SZ_REPOS               XTP_ATTR_REPOS
#define SZ_HORRESIZE           XTP_ATTR_HORRESIZE
#define SZ_HORREPOS            XTP_ATTR_HORREPOS
#define SZ_VERRESIZE           XTP_ATTR_VERRESIZE
#define SZ_VERREPOS            XTP_ATTR_VERREPOS
#define SZ_BOTTOM_CENTER       XTP_ANCHOR_BOTTOMCENTER
#define SZ_BOTTOM_LEFT         XTP_ANCHOR_BOTTOMLEFT
#define SZ_BOTTOM_RIGHT        XTP_ANCHOR_BOTTOMRIGHT
#define SZ_MIDDLE_CENTER       XTP_ANCHOR_MIDDLECENTER
#define SZ_MIDDLE_LEFT         XTP_ANCHOR_MIDDLELEFT
#define SZ_MIDDLE_RIGHT        XTP_ANCHOR_MIDDLERIGHT
#define SZ_TOP_LEFT            XTP_ANCHOR_TOPLEFT
#define SZ_TOP_RIGHT           XTP_ANCHOR_TOPRIGHT
#define SZ_TOP_CENTER          XTP_ANCHOR_TOPCENTER

#define XT_IDC_LBOX_EDIT       XTP_IDC_LBOX_EDIT
#define XT_IDC_BTN_CLOSE       XTP_IDC_BTN_CLOSE
#define XT_IDC_TAB_CLOSE       XTP_IDC_TAB_CLOSE
#define XT_IDB_BTN_OUT         XTP_IDB_BTN_OUT

#define BES_XT_CHOOSEDIR       BES_XTP_CHOOSEDIR
#define BES_XT_CHOOSEFILE      BES_XTP_CHOOSEFILE
#define BES_XT_POPUPMENU       BES_XTP_POPUPMENU
#define BES_XT_BROWSE          BES_XTP_BROWSE
#define CPS_XT_NOFILL          CPS_XTP_NOFILL
#define CPS_XT_EXTENDED        CPS_XTP_EXTENDED
#define CPS_XT_MORECOLORS      CPS_XTP_MORECOLORS
#define CPS_XT_SHOW3DSELECTION CPS_XTP_SHOW3DSELECTION
#define CPS_XT_SHOWHEXVALUE    CPS_XTP_SHOWHEXVALUE
#define CPS_XT_SHOWEYEDROPPER  CPS_XTP_SHOWEYEDROPPER
#define LBS_XT_DEFAULT         LBS_XTP_DEFAULT
#define LBS_XT_CHOOSEDIR       LBS_XTP_CHOOSEDIR
#define LBS_XT_CHOOSEFILE      LBS_XTP_CHOOSEFILE
#define LBS_XT_NOTOOLBAR       LBS_XTP_NOTOOLBAR
#define LBS_XT_BROWSE          LBS_XTP_BROWSE
#define LBS_XT_HIDE_UP_DOWN    LBS_XTP_HIDE_UP_DOWN
#define LBS_XT_ONLY_UP_DOWN    LBS_XTP_ONLY_UP_DOWN
#define LBS_XT_BROWSE_ONLY     LBS_XTP_BROWSE_ONLY
#define FTS_XT_BOTTOM          FTS_XTP_BOTTOM
#define FTS_XT_HASARROWS       FTS_XTP_HASARROWS
#define FTS_XT_HASHOMEEND      FTS_XTP_HASHOMEEND
#define FTS_XT_TOOLTIPS        FTS_XTP_TOOLTIPS
#define FTS_XT_DEFAULT         FTS_XTP_DEFAULT
#define FTS_XT_HSCROLL         FTS_XTP_HSCROLL
#define FTS_XT_MASK            FTS_XTP_MASK

#define CPN_XT_SELCHANGE          CPN_XTP_SELCHANGE
#define ON_CPN_XT_SELCHANGE       ON_CPN_XTP_SELCHANGE
#define CPN_XT_DROPDOWN           CPN_XTP_DROPDOWN
#define ON_CPN_XT_DROPDOWN        ON_CPN_XTP_DROPDOWN
#define CPN_XT_CLOSEUP            CPN_XTP_CLOSEUP
#define ON_CPN_XT_CLOSEUP         ON_CPN_XTP_CLOSEUP
#define CPN_XT_SELENDOK           CPN_XTP_SELENDOK
#define ON_CPN_XT_SELENDOK        ON_CPN_XTP_SELENDOK
#define CPN_XT_SELENDCANCEL       CPN_XTP_SELENDCANCEL
#define ON_CPN_XT_SELENDCANCEL    ON_CPN_XTP_SELENDCANCEL
#define CPN_XT_SELNOFILL          CPN_XTP_SELNOFILL
#define ON_CPN_XT_SELNOFILL       ON_CPN_XTP_SELNOFILL
#define BEN_XT_LABELEDITEND       BEN_XTP_LABELEDITEND
#define ON_BEN_XT_LABELEDITEND    ON_BEN_XTP_LABELEDITEND
#define BEN_XT_LABELEDITCANCEL    BEN_XTP_LABELEDITCANCEL
#define ON_BEN_XT_LABELEDITCANCEL ON_BEN_XTP_LABELEDITCANCEL
#define LBN_XT_LABELEDITEND       LBN_XTP_LABELEDITEND
#define ON_LBN_XT_LABELEDITEND    ON_LBN_XTP_LABELEDITEND
#define LBN_XT_LABELEDITCANCEL    LBN_XTP_LABELEDITCANCEL
#define ON_LBN_XT_LABELEDITCANCEL ON_LBN_XTP_LABELEDITCANCEL
#define LBN_XT_NEWITEM            LBN_XTP_NEWITEM
#define ON_LBN_XT_NEWITEM         ON_LBN_XTP_NEWITEM
#define LBN_XT_PREDELETEITEM      LBN_XTP_PREDELETEITEM
#define ON_LBN_XT_PREDELETEITEM   ON_LBN_XTP_PREDELETEITEM
#define LBN_XT_DELETEITEM         LBN_XTP_DELETEITEM
#define ON_LBN_XT_DELETEITEM      ON_LBN_XTP_DELETEITEM
#define LBN_XT_MOVEITEMUP         LBN_XTP_MOVEITEMUP
#define ON_LBN_XT_MOVEITEMUP      ON_LBN_XTP_MOVEITEMUP
#define LBN_XT_MOVEITEMDOWN       LBN_XTP_MOVEITEMDOWN
#define ON_LBN_XT_MOVEITEMDOWN    ON_LBN_XTP_MOVEITEMDOWN
#define LBN_XT_PRENEWITEM         LBN_XTP_PRENEWITEM
#define ON_LBN_XT_PRENEWITEM      ON_LBN_XTP_PRENEWITEM
#define LBN_XT_ONBROWSE           LBN_XTP_ONBROWSE
#define ON_LBN_XT_ONBROWSE        ON_LBN_XTP_ONBROWSE
#define SON_XT_ITEMEXPANDING      SON_XTP_ITEMEXPANDING
#define ON_SON_XT_ITEMEXPANDING   ON_SON_XTP_ITEMEXPANDING
#define SON_XT_ITEMEXPAND         SON_XTP_ITEMEXPAND
#define ON_SON_XT_ITEMEXPAND      ON_SON_XTP_ITEMEXPAND
#define SON_XT_ITEMCONTRACTING    SON_XTP_ITEMCONTRACTING
#define ON_SON_XT_ITEMCONTRACTING ON_SON_XTP_ITEMCONTRACTING
#define SON_XT_ITEMCONTRACT       SON_XTP_ITEMCONTRACT
#define ON_SON_XT_ITEMCONTRACT    ON_SON_XTP_ITEMCONTRACT

#define XTWM_OUTBAR_NOTIFY      XTWM_OUTBAR_NOTIFY
#define OBN_XT_ITEMCLICK        OBN_XTP_ITEMCLICK
#define OBN_XT_ONLABELENDEDIT   OBN_XTP_ONLABELENDEDIT
#define OBN_XT_ONGROUPENDEDIT   OBN_XTP_ONGROUPENDEDIT
#define OBN_XT_DRAGITEM         OBN_XTP_DRAGITEM
#define OBN_XT_FOLDERCHANGE     OBN_XTP_FOLDERCHANGE
#define OBN_XT_ITEMHOVER        OBN_XTP_ITEMHOVER
#define OBN_XT_DELETEITEM       OBN_XTP_DELETEITEM
#define OBN_XT_DELETEFOLDER     OBN_XTP_DELETEFOLDER
#define OBN_XT_BEGINDRAG        OBN_XTP_BEGINDRAG
#define OBN_XT_ITEMRCLICK       OBN_XTP_ITEMRCLICK

#define XTWM_SHELL_NOTIFY      WM_XTP_SHELL_NOTIFY
#define SHN_XT_SHELLMENU       SHN_XTP_SHELLMENU
#define SHN_XT_TREESELCHANGE   SHN_XTP_TREESELCHANGE
#define SHN_XT_SELECTCHILD     SHN_XTP_SELECTCHILD
#define SHN_XT_NOFOLDER        SHN_XTP_NOFOLDER
#define SHN_XT_INETFOLDER      SHN_XTP_INETFOLDER
#define SHN_XT_CONTENTSCHANGED SHN_XTP_CONTENTSCHANGED
#define SHN_XT_REFRESHFOLDER   SHN_XTP_REFRESHFOLDER
#define SHN_XT_REFRESHTREE     SHN_XTP_REFRESHTREE

#define TIN_XT_TRAYICON        TIN_XTP_TRAYICON
#define CPWN_XT_PUSHPINBUTTON  CPN_XTP_PUSHPINBUTTON
#define CPWN_XT_PUSHPINCANCEL  CPN_XTP_PUSHPINCANCEL
#define TCN_XT_SELCHANGE       TCN_SELCHANGE
#define TCN_XT_SELCHANGING     TCN_SELCHANGING
#define XTWM_UPDATECOLOR       WM_XTP_UPDATECOLOR
#define XTWM_COLORDBLCLK       WM_XTP_COLORDBLCLK
#define XTWM_EYEDROPPER_MOVING WM_XTP_EYEDROPPER_MOVING
#define XTWM_EYEDROPPER_SELECT WM_XTP_EYEDROPPER_SELECT

#define XTTHEME_WINXPTHEMES    HDR_XTP_WINDEF
#define XTTHEME_HOTTRACKING    HDR_XTP_HOTTRACKING
#define XTTHEME_SORTARROW      HDR_XTP_SORTARROW

#define XT_SPLIT_DOTTRACKER    XTP_SPLIT_DOTTRACKER
#define XT_SPLIT_NOFULLDRAG    XTP_SPLIT_NOFULLDRAG
#define XT_SPLIT_NOBORDER      XTP_SPLIT_NOBORDER
#define XT_SPLIT_NOSIZE        XTP_SPLIT_NOSIZE

#define TWS_XT_THICKBORDER     TWS_XTP_THICKBORDER
#define TWS_XT_DROPSHADOW      TWS_XTP_DROPSHADOW
#define TWS_XT_ALPHASHADOW     TWS_XTP_ALPHASHADOW

//{{AFX_CODEJOCK_PRIVATE
// -------------------
// deprecated includes
// -------------------
#include "../Deprecated/XTVC50Helpers.h"
#include "../Deprecated/XTThemeManager.h"
#include "../Deprecated/XTButton.h"
#include "../Deprecated/XTButtonTheme.h"
#include "../Deprecated/XTColorRef.h"
#include "../Deprecated/XTComboBoxEx.h"
#include "../Deprecated/XTDateTimeCtrl.h"
#include "../Deprecated/XTFlatComboBox.h"
#include "../Deprecated/XTFlatControlsTheme.h"
#include "../Deprecated/XTHtmlView.h"
#include "../Deprecated/XTMemDC.h"
#include "../Deprecated/XTOutBarCtrl.h"
#include "../Deprecated/XTOutBarCtrlTheme.h"

#endif //#if !defined(__XTPCONTROLS_DEPRECATED_H__)
//}}AFX_CODEJOCK_PRIVATE
