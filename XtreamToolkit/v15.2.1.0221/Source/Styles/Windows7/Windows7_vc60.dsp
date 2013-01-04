# Microsoft Developer Studio Project File - Name="Windows7" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Windows7 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Windows7.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Windows7.mak" CFG="Windows7 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Windows7 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release/vc60"
# PROP BASE Intermediate_Dir "Release/vc60"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release/vc60"
# PROP Intermediate_Dir "Release/vc60"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCEARSA_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCE_EXPORTS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../" /d "NDEBUG" /d "_AFXDLL" /d "_XTP_INCLUDE_VERSION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Windows7.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "Windows7 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Windows7.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Blue_ContextTabBlue.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabBlueClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabBlueGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabBlueHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabCyan.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabCyanClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabCyanGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabCyanHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGreen.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGreenClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGreenGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGreenHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabOrange.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabOrangeClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabOrangeGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabOrangeHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabPurple.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabPurpleClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabPurpleGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabPurpleHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabRed.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabRedClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabRedGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabRedHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabYellow.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabYellowClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabYellowGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ContextTabYellowHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryPopup.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowsVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowsVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameBottomLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameBottomRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionClose13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionClose17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionClose23.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMaximize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMaximize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMaximize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMinimize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMinimize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMinimize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionRestore13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionRestore17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionRestore23.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameSimpleTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameSystemButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameTopCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuCheckedItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuCheckedItemMark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuSelectedDisabledItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuSelectedDisabledItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuSelectedItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuSelectedItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupButtonIcon.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupQuickAccess.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroups.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupsScrollLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupsScrollLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupsScrollRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonGroupsScrollRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonOptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonQuickAccessButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonQuickAccessFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonQuickAccessMore.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBarDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBarDevider.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBarLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBarSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonCheckBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtons22.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtons50.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsComboDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSingle.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitDropDownCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitDropDownLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitDropDownRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSpecialSplitRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplit22.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplit50.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplitDropDown22.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplitDropDown50.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ReportExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_WorkspaceTopLeft.png
# End Source File
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\BlueTheme.ini
# End Source File
# End Group
# End Target
# End Project
