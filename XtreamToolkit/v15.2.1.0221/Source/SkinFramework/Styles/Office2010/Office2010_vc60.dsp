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
# ADD CPP /nologo /MD /W3 /GX /O1 /I "../../../Source" /I "../../../Source/Common" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XTPRESOURCE_EXPORTS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../../../" /d "NDEBUG" /d "_AFXDLL" /d "_XTP_INCLUDE_VERSION"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386 /nodefaultlib /out:"../Office2010.cjstyles" /opt:nowin98 /noentry
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

# PROP Default_Filter "bmp"
# Begin Source File

SOURCE=.\res\Aqua_Button.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_CaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_CheckBox13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_CloseGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_CloseGlyph19.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ComboButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ComboButtonGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameBottom.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_FrameRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_GroupBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_HelpGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_HelpGlyph19.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ListViewHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ListviewHeaderBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MaximizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MaximizeGlyph19.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MinimizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_MinimizeGlyph19.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ProgressChunkVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ProgressTrackVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RadioButton13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ResizeGrip2.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RestoreGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_RestoreGlyph19.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_Separator.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SeparatorVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_sliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SmallFrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinButtonBackgroundDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinButtonBackgroundLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinButtonBackgroundRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinButtonBackgroundUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_SpinUpGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_StatusPane.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TabBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TabBackground133.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TabItemTop.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TabItemTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TabPaneEdge.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtons.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplit.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplitDropdown.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarButtonsSplitDropdownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_ToolbarGripperVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackBarDown13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackbarHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackBarLeft13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackbarRight13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackBarUp13.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TrackbarVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TreeExpandCollapse.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TreeExpandCollapse10.png
# End Source File
# Begin Source File

SOURCE=.\res\Aqua_TreeExpandCollapse15.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_Button.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CheckBox13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CloseGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_CloseGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ComboButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ComboButtonGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameBottom.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionMin.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameCaptionSizing.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameMaximized.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_FrameRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_GroupBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_HelpGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_HelpGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ListViewHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ListviewHeaderBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MaximizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MaximizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MinimizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_MinimizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressChunkVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ProgressTrackVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RadioButton13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ResizeGrip2.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RestoreGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_RestoreGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollArrows.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollShaftHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollShaftVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_Separator.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SeparatorVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_sliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SmallFrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinButtonBackgroundDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinButtonBackgroundLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinButtonBackgroundRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinButtonBackgroundUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_SpinUpGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_StatusPane.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TabBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TabItemTop.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TabItemTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TabPaneEdge.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtons.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplit.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplitDropdown.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarButtonsSplitDropdownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_ToolbarGripperVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackBarDown13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackbarHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackBarLeft13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackbarRight13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackBarUp13.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TrackbarVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TreeExpandCollapse.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TreeExpandCollapse10.png
# End Source File
# Begin Source File

SOURCE=.\res\Black_TreeExpandCollapse15.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_Button.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CheckBox13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CloseGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_CloseGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ComboButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ComboButtonGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameBottom.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionMin.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameCaptionSizing.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameMaximized.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_FrameRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_GroupBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_HelpGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_HelpGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ListViewHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ListviewHeaderBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MaximizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MaximizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MinimizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_MinimizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressChunkVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ProgressTrackVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RadioButton13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ResizeGrip2.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RestoreGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_RestoreGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollArrows.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollShaftHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollShaftVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_Separator.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SeparatorVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_sliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SmallFrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinButtonBackgroundDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinButtonBackgroundLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinButtonBackgroundRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinButtonBackgroundUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_SpinUpGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_StatusPane.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TabBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TabItemTop.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TabItemTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TabPaneEdge.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtons.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplit.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplitDropdown.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarButtonsSplitDropdownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_ToolbarGripperVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackBarDown13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackbarHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackBarLeft13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackbarRight13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackBarUp13.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TrackbarVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TreeExpandCollapse.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TreeExpandCollapse10.png
# End Source File
# Begin Source File

SOURCE=.\res\Blue_TreeExpandCollapse15.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_Button.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_CaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_CheckBox13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_CloseGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_CloseGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ComboButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ComboButtonGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameBottom.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameCaptionMin.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameCaptionSizing.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameMaximized.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_FrameRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_GroupBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_HelpGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_HelpGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ListViewHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ListViewHeaderBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_MaximizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_MaximizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_MinimizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_MinimizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ProgressChunkVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ProgressTrackVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_RadioButton13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ResizeGrip2.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_RestoreGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_RestoreGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollArrows.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollShaftHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollShaftVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_Separator.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SeparatorVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SmallFrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinButtonBackgroundDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinButtonBackgroundLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinButtonBackgroundRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinButtonBackgroundUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_SpinUpGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_StatusBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_StatusPane.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TabBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TabItemTop.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TabItemTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TabPaneEdge.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarButtons.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarButtonsSplit.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarButtonsSplitDropDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarButtonsSplitDropDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_ToolbarGripperVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarDown13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarLeft13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarRight13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarUp13.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TrackbarVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TreeExpandCollapse.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TreeExpandCollapse10.png
# End Source File
# Begin Source File

SOURCE=.\res\Office2010_TreeExpandCollapse15.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_Button.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CaptionButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CheckBox13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CloseGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_CloseGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ComboButton.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ComboButtonGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameBottom.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionMin.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameCaptionSizing.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameMaximized.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_FrameRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_GroupBox.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_HelpGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_HelpGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ListViewHeader.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ListviewHeaderBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MaximizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MaximizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MinimizeGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_MinimizeGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressChunk.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressChunkVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ProgressTrackVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RadioButton13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ResizeGrip2.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RestoreGlyph13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_RestoreGlyph17.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollArrowGlyphs.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollArrows.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollShaftHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollShaftVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollThumbGripperHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollThumbGripperVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollThumbHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ScrollThumbVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_Separator.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SeparatorVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_sliderTrack.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SmallFrameCaption.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinButtonBackgroundDown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinButtonBackgroundLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinButtonBackgroundRight.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinButtonBackgroundUp.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinDownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinLeftGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinRightGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_SpinUpGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_StatusPane.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TabBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TabItemTop.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TabItemTopLeft.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TabPaneEdge.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarBackground.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtons.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplit.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplitDropdown.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarButtonsSplitDropdownGlyph.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarGripper.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_ToolbarGripperVert.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackBarDown13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackbarHorizontal.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackBarLeft13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackbarRight13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackBarUp13.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TrackbarVertical.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TreeExpandCollapse.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TreeExpandCollapse10.png
# End Source File
# Begin Source File

SOURCE=.\res\Silver_TreeExpandCollapse15.png
# End Source File
# End Group
# Begin Group "Ini Files"

# PROP Default_Filter "ini"
# Begin Source File

SOURCE=.\res\NormalAqua.ini
# End Source File
# Begin Source File

SOURCE=.\res\NormalBlack.ini
# End Source File
# Begin Source File

SOURCE=.\res\NormalBlue.ini
# End Source File
# Begin Source File

SOURCE=.\res\NormalSilver.ini
# End Source File
# Begin Source File

SOURCE=.\res\Themes.ini
# End Source File
# End Group
# End Target
# End Project
