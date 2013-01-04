# Microsoft Developer Studio Project File - Name="Office2010" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Office2010 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Office2010.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Office2010.mak" CFG="Office2010 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Office2010 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Office2010.dll" /opt:nowin98 /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "Office2010 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Office2010.rc
# End Source File
# End Group
# Begin Group "Image Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Aqua_ContextTabBlue.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabBlueClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabBlueGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabBlueHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabCyan.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabCyanClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabCyanGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabCyanHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGreen.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGreenClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGreenGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGreenHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabOrange.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabOrangeClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabOrangeGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabOrangeHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabPurple.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabPurpleClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabPurpleGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabPurpleHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabRed.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabRedClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabRedGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabRedHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabYellow.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabYellowClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabYellowGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ContextTabYellowHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryPopup.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollArrowsVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollArrowsVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryScrollVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ControlGalleryUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameBottomLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameBottomRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionClose13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionClose17.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionClose23.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMaximize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMaximize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMaximize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMinimize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMinimize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionMinimize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionRestore13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionRestore17.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaptionRestore23.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameSimpleTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameSystemButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameTopCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuCheckedItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuCheckedItemMark.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuSelectedDisabledItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuSelectedDisabledItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuSelectedItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MenuSelectedItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_PopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupButtonIcon.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupQuickAccess.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroups.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupsScrollLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupsScrollLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupsScrollRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonGroupsScrollRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonOptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonQuickAccessButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonQuickAccessFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonQuickAccessMore.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RibbonTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBarDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBarDevider.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBarLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBarSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonCheckBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtons22.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtons50.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsComboDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSingle.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitDropDownCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitDropDownLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitDropDownRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSpecialSplitRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplit22.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplit50.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplitDropDown22.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplitDropDown50.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_WorkspaceTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_AccessTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarCaptionBarExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarCaptionBarNextDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarCaptionBarPrevDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarCaptionBarSwitchViewButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarHeader.PNG
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarHeaderDVGroup.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarHeaderMVDay.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarHeaderMVWeek.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarHeaderWVDay.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarPrevNextEventButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CalendarPrevNextEventButtonArr.PNG
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabBlue.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabBlueClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabBlueGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabBlueHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabCyan.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabCyanClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabCyanGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabCyanHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGreen.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGreenClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGreenGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGreenHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabOrange.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabOrangeClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabOrangeGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabOrangeHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabPurple.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabPurpleClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabPurpleGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabPurpleHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabRed.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabRedClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabRedGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabRedHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabYellow.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabYellowClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabYellowGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ContextTabYellowHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryPopup.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollArrowsHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollArrowsHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollArrowsVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollArrowsVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryScrollVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ControlGalleryUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FileButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameBottomLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameBottomRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionClose13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionClose17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionClose23.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMaximize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMaximize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMaximize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMinimize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMinimize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMinimize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionRestore13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionRestore17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionRestore23.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameSystemButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameSystemButton52.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameTopCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ListBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuCheckedItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuCheckedItemMark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuPopupGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSelectedDisabledItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSelectedDisabledItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSelectedItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSelectedItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSplitDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MenuSplitItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MorePopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_PopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_PopupBarResizeGripperFace.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_PopupBarResizeGripperHeight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_PopupBarResizeGripperWidth.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_PushButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ReportExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupButtonIcon.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupQuickAccess.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroups.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupsScrollLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupsScrollLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupsScrollRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonGroupsScrollRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonOptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonQuickAccessArea.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonQuickAccessAreaDwm.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonQuickAccessButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonQuickAccessFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonQuickAccessMore.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonSystemMenu.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonSystemMenuButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RibbonTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ShortcutBarGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ShortcutBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ShortcutBarItems.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ShortcutBarNavigateItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SliderDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SliderThumb.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SliderTick.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SliderUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarDevider.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBarSwitches.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonCheckBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonRadioButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtons22.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtons50.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsComboDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonSpinArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonSpinArrowsHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonSpinArrowsVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSingle.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitDropDownCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitDropDownLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitDropDownRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSpecialSplitRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplit22.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplit50.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplitDropDown22.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplitDropDown50.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarExpandButtonHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarExpandButtonVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarFaceHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarFaceVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TooltipFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_WorkspaceTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_AccessTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarCaptionBarExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarCaptionBarNextDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarCaptionBarPrevDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarCaptionBarSwitchViewButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarPrevNextEventButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CalendarPrevNextEventButtonArr.PNG
# End Source File
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

SOURCE=.\res\Blue_ControlGalleryScrollArrowsHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowsHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowsVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollArrowsVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ControlGalleryScrollThumbHorizontal.png
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

SOURCE=.\res\Blue_FileButton.png
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

SOURCE=.\res\Blue_FrameSystemButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameSystemButton52.png
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

SOURCE=.\res\Blue_ListBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuCheckedItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuCheckedItemMark.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuPopupGlyph.png
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

SOURCE=.\res\Blue_MenuSplitDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MenuSplitItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MorePopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PopupBarResizeGripperFace.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PopupBarResizeGripperHeight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PopupBarResizeGripperWidth.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_PushButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ReportExpandButton.png
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

SOURCE=.\res\Blue_RibbonQuickAccessArea.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonQuickAccessAreaDwm.png
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

SOURCE=.\res\Blue_RibbonSystemMenu.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonSystemMenuButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RibbonTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ShortcutBarGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ShortcutBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ShortcutBarItems.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ShortcutBarNavigateItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SliderDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SliderThumb.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SliderTick.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SliderUp.png
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

SOURCE=.\res\Blue_StatusBarSwitches.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonCheckBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonRadioButton.png
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

SOURCE=.\res\Blue_ToolbarButtonSpinArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonSpinArrowsHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonSpinArrowsVertical.png
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

SOURCE=.\res\Blue_ToolbarExpandButtonHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarExpandButtonVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarFaceHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarFaceVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TooltipFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_WorkspaceTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_AccessTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarCaptionBarExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarCaptionBarNextDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarCaptionBarPrevDateButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarCaptionBarSwitchViewButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarHeader.PNG
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarHeaderDVGroup.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarHeaderMVDay.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarHeaderMVWeek.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarHeaderWVDay.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarPrevNextEventButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CalendarPrevNextEventButtonArr.PNG
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabBlue.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabBlueClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabBlueGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabBlueHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabCyan.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabCyanClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabCyanGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabCyanHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGreen.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGreenClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGreenGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGreenHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabOrange.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabOrangeClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabOrangeGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabOrangeHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabPurple.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabPurpleClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabPurpleGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabPurpleHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabRed.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabRedClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabRedGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabRedHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabYellow.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabYellowClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabYellowGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ContextTabYellowHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryPopup.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollArrowsHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollArrowsHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollArrowsVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollArrowsVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollHorizontalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollHorizontalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollVerticalDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryScrollVerticalLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ControlGalleryUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FileButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameBottomLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameBottomRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionClose13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionClose17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionClose23.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMaximize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMaximize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMaximize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMinimize13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMinimize17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMinimize23.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionRestore13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionRestore17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionRestore23.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameSystemButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameSystemButton52.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameTopCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameTopRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ListBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuCheckedItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuCheckedItemMark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuPopupGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSelectedDisabledItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSelectedDisabledItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSelectedItem22.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSelectedItem54.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSplitDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MenuSplitItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MorePopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_PopupBarFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_PopupBarResizeGripperFace.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_PopupBarResizeGripperHeight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_PopupBarResizeGripperWidth.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_PushButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ReportExpandButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupButtonIcon.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupClient.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupQuickAccess.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroups.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupsScrollLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupsScrollLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupsScrollRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonGroupsScrollRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonOptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonQuickAccessArea.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonQuickAccessAreaDwm.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonQuickAccessButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonQuickAccessFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonQuickAccessMore.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonSystemMenu.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonSystemMenuButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonTab.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RibbonTabSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ShortcutBarGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ShortcutBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ShortcutBarItems.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ShortcutBarNavigateItem.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SliderDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SliderThumb.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SliderTick.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SliderUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarDark.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarDevider.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarLight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarSeparator.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBarSwitches.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonCheckBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonRadioButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtons22.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtons50.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsComboDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonSpinArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonSpinArrowsHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonSpinArrowsVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSingle.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitDropDownCenter.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitDropDownLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitDropDownRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSpecialSplitRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplit22.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplit50.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplitDropDown22.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplitDropDown50.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarExpandButtonHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarExpandButtonVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarFaceHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarFaceVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TooltipFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_WorkspaceTopLeft.png
# End Source File
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\AquaTheme.ini
# End Source File
# Begin Source File

SOURCE=.\res\BlackTheme.ini
# End Source File
# Begin Source File

SOURCE=.\res\BlueTheme.ini
# End Source File
# Begin Source File

SOURCE=.\res\SilverTheme.ini
# End Source File
# End Group
# Begin Source File

SOURCE=.\Styles\Office2010\res\AquaTheme.ini
# End Source File
# Begin Source File

SOURCE=.\Styles\Office2010\res\Silver_SliderTick.png
# End Source File
# Begin Source File

SOURCE=.\Styles\Office2010\res\SilverTheme.ini
# End Source File
# End Target
# End Project
