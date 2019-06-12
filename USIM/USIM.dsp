# Microsoft Developer Studio Project File - Name="USIM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=USIM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "USIM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "USIM.mak" CFG="USIM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "USIM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "USIM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "USIM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Gz /MD /W4 /GX /O2 /I "ResizableLib" /I "XPPlugin\SDK\CHeaders\XPLM" /I "libtree" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "LIB_IMPORTS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "COptionTree\CommonRes.rc" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib wininet.lib winmm.lib shlwapi.lib setupapi.lib hid.lib biusb.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /libpath:"hid_dll"
# SUBTRACT LINK32 /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "USIM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gz /MDd /W4 /Gm /GX /ZI /Od /I "ResizableLib" /I "XPPlugin\SDK\CHeaders\XPLM" /I "libtree" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "LIB_IMPORTS" /D "_AFX_ENABLE_INLINES" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x1009 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /i "COptionTree\CommonRes.rc" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib wininet.lib winmm.lib shlwapi.lib setupapi.lib hid.lib biusb.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /pdbtype:sept /libpath:"hid_dll"

!ENDIF 

# Begin Target

# Name "USIM - Win32 Release"
# Name "USIM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "NewMenu"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\NewMenu\NewMenu.cpp
# End Source File
# End Group
# Begin Group "ColorTreeCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorTreeCtrl\TreeCtrlEx.cpp
# End Source File
# End Group
# Begin Group "ReadOnlyEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ReadOnlyEdit\ReadOnlyEdit.cpp
# End Source File
# End Group
# Begin Group "XTabCtrl"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\XTabCtrl\XTabCtrl.cpp
# End Source File
# End Group
# Begin Group "Output"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\CalSPI.cpp
# End Source File
# Begin Source File

SOURCE=.\CalSTEP.cpp
# End Source File
# Begin Source File

SOURCE=.\CalSTEP.h
# End Source File
# Begin Source File

SOURCE=.\CLCDFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDLinesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDLinesTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDTab.cpp
# End Source File
# Begin Source File

SOURCE=.\CLCDTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DispFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DispNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DispTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DispTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DotMXFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DotMXNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DotMXPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DotMXTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DotMXTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FlagsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDLinesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDLinesTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDTab.cpp
# End Source File
# Begin Source File

SOURCE=.\GLCDTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LatchedFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\LatchedNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\LatchedTab.cpp
# End Source File
# Begin Source File

SOURCE=.\LatchedTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MuxFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MuxNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MuxTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MuxTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputTab.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RWRFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\RWRNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\RwrPage.cpp
# End Source File
# Begin Source File

SOURCE=.\RWRTab.cpp
# End Source File
# Begin Source File

SOURCE=.\RWRTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\SPINotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\SPIPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SPITab.cpp
# End Source File
# Begin Source File

SOURCE=.\SPITabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPFlagsTab.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\STEPNotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\STEPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPPage.h
# End Source File
# Begin Source File

SOURCE=.\STEPTab.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPTab.h
# End Source File
# Begin Source File

SOURCE=.\STEPTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\STEPTabCtrl.h
# End Source File
# End Group
# Begin Group "amsEdit"

# PROP Default_Filter "C"
# Begin Source File

SOURCE=.\amsEdit\amsEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\amsEdit\amsEdit.h
# End Source File
# End Group
# Begin Group "CEAdvButton"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\CEAdvButton\CEAdvButton.cpp
# End Source File
# End Group
# Begin Group "ButtonSSL"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\ButtonSSL\ButtonSSL.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonSSL\OddButton.cpp
# End Source File
# End Group
# Begin Group "SmartEdit"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\SmartEdit\Sliders.cpp
# End Source File
# Begin Source File

SOURCE=.\SmartEdit\SmartEdit.cpp
# End Source File
# End Group
# Begin Group "DefTabCtrl"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\DefaultTab.cpp
# End Source File
# Begin Source File

SOURCE=.\DefTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SimPathTab.cpp
# End Source File
# End Group
# Begin Group "UpdateCheck"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\UpdateCheck\UpdateCheck.cpp
# End Source File
# End Group
# Begin Group "ThreadClass"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\IL2_UDPThread.cpp
# End Source File
# Begin Source File

SOURCE=.\InputThread.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyCheckThread.cpp
# End Source File
# Begin Source File

SOURCE=.\LFS_UDPThread.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SendKeysThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SimConnectThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadClass\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeThread.cpp
# End Source File
# End Group
# Begin Group "PathSplit"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\PathSplit\CPathSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\PathSplit\CPathSplit.h
# End Source File
# End Group
# Begin Group "GridCtrl"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.cpp
# End Source File
# End Group
# Begin Group "NewCellTypes"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellRadio.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridURLCell.cpp
# End Source File
# End Group
# Begin Group "Version"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Version\Version.cpp
# End Source File
# End Group
# Begin Group "HexEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HexEdit\HexEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEdit\HexEdit.h
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InputFlagTab.cpp
# End Source File
# Begin Source File

SOURCE=.\InputsTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NotesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\NotesTab.cpp
# End Source File
# Begin Source File

SOURCE=.\SendKeysPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SendKeysTab.cpp
# End Source File
# Begin Source File

SOURCE=.\VariablesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\VariablesTab.cpp
# End Source File
# End Group
# Begin Group "ResizableLib"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\ResizableLib\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableGrip.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableMinMax.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableState.cpp
# End Source File
# End Group
# Begin Group "xySplitterWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xySplitterWnd\xySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\xySplitterWnd\xySplitterWnd.h
# End Source File
# End Group
# Begin Group "TrayDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrayDialog\TrayDialog.cpp
# End Source File
# End Group
# Begin Group "w32process"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\w32process\w32process.cpp
# End Source File
# End Group
# Begin Group "NumSpinCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NumSpinCtrl\NumSpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NumSpinCtrl\NumSpinCtrl.h
# End Source File
# End Group
# Begin Group "libtree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libtree\editmask.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\editmask.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTree.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTree.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeCheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeCheckButton.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeColorPopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeColorPopUp.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeDef.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSel.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSel.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelColorButton.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelFontCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelFontCombo.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelGlobals.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelGlobals.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelSizeCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeFontSelSizeCombo.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeImagePopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeImagePopUp.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeInfo.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeIPAddressEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeIPAddressEdit.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItem.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemColor.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemColor.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemComboBox.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemDate.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemDate.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemFile.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemFile.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemFont.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemFont.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemImage.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemImage.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemIPAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemIPAddress.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemRadio.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemRadio.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemSpinner.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemSpinner.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeItemStatic.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeList.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeList.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeSpinnerButton.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeSpinnerButton.h
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeSpinnerEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\OptionTreeSpinnerEdit.h
# End Source File
# Begin Source File

SOURCE=.\libtree\PreciseTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\PreciseTimer.h
# End Source File
# Begin Source File

SOURCE=.\libtree\regexp.cpp
# End Source File
# Begin Source File

SOURCE=.\libtree\regexp.h
# End Source File
# End Group
# Begin Group "FileOperations"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileOperations\FileOperations.cpp
# End Source File
# Begin Source File

SOURCE=.\FileOperations\FileOperations.h
# End Source File
# End Group
# Begin Group "FileEdit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\FileEdit.h
# End Source File
# End Group
# Begin Group "SimplexParser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SimplexParser\FormelParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplexParser\FormelParser.h
# End Source File
# Begin Source File

SOURCE=.\SimplexParser\PhysConst.cpp
# End Source File
# Begin Source File

SOURCE=.\SimplexParser\PhysConst.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\F4DataOut.cpp
# End Source File
# Begin Source File

SOURCE=.\F4SharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\GPXSharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyboardHook.cpp
# End Source File
# Begin Source File

SOURCE=.\MyStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\RFSharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveUserVars.cpp
# End Source File
# Begin Source File

SOURCE=.\SimBinSharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TreeDevices.cpp

!IF  "$(CFG)" == "USIM - Win32 Release"

!ELSEIF  "$(CFG)" == "USIM - Win32 Debug"

# ADD CPP /W4

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\USIM.cpp
# End Source File
# Begin Source File

SOURCE=.\USIMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VarListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XPSharedMem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "NewMenu_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewMenu\NewMenu.h
# End Source File
# End Group
# Begin Group "ColorTreeCtrl_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorTreeCtrl\TreeCtrlEx.h
# End Source File
# End Group
# Begin Group "ReadOnlyEdit_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Format.h
# End Source File
# Begin Source File

SOURCE=.\ReadOnlyEdit\ReadOnlyEdit.h
# End Source File
# End Group
# Begin Group "XTabCtrl_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\XTabCtrl\XTabCtrl.h
# End Source File
# End Group
# Begin Group "Output_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CalSPI.h
# End Source File
# Begin Source File

SOURCE=.\CLCDFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\CLCDLinesTab.h
# End Source File
# Begin Source File

SOURCE=.\CLCDLinesTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CLCDNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\CLCDPage.h
# End Source File
# Begin Source File

SOURCE=.\CLCDTab.h
# End Source File
# Begin Source File

SOURCE=.\CLCDTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DispFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\DisplayPage.h
# End Source File
# Begin Source File

SOURCE=.\DispNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\DispTab.h
# End Source File
# Begin Source File

SOURCE=.\DispTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DotMXFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\DotMXNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\DotMXPage.h
# End Source File
# Begin Source File

SOURCE=.\DotMXTab.h
# End Source File
# Begin Source File

SOURCE=.\DotMXTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FlagsPage.h
# End Source File
# Begin Source File

SOURCE=.\FlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\GLCDFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\GLCDLinesTab.h
# End Source File
# Begin Source File

SOURCE=.\GLCDLinesTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GLCDNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\GLCDTab.h
# End Source File
# Begin Source File

SOURCE=.\GLCDTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LatchedFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\LatchedNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\LatchedTab.h
# End Source File
# Begin Source File

SOURCE=.\LatchedTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MuxFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\MuxNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\MuxTab.h
# End Source File
# Begin Source File

SOURCE=.\MuxTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OutputFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\OutputNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\OutputPage.h
# End Source File
# Begin Source File

SOURCE=.\OutputTab.h
# End Source File
# Begin Source File

SOURCE=.\OutputTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RWRFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\RWRNotesTab.h
# End Source File
# Begin Source File

SOURCE=.\RwrPage.h
# End Source File
# Begin Source File

SOURCE=.\RWRTab.h
# End Source File
# Begin Source File

SOURCE=.\RWRTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SPIFlagsTab.h
# End Source File
# Begin Source File

SOURCE=.\SPINotesTab.h
# End Source File
# Begin Source File

SOURCE=.\SPIPage.h
# End Source File
# Begin Source File

SOURCE=.\SPITab.h
# End Source File
# Begin Source File

SOURCE=.\SPITabCtrl.h
# End Source File
# End Group
# Begin Group "CEAdvButton_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\CEAdvButton\CEAdvButton.h
# End Source File
# End Group
# Begin Group "ButtonSSL_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\ButtonSSL\ButtonSSL.h
# End Source File
# Begin Source File

SOURCE=.\ButtonSSL\OddButton.h
# End Source File
# End Group
# Begin Group "SmartEdit_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\SmartEdit\Sliders.h
# End Source File
# Begin Source File

SOURCE=.\SmartEdit\SmartEdit.h
# End Source File
# End Group
# Begin Group "DefTabCtrl_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\DefaultTab.h
# End Source File
# Begin Source File

SOURCE=.\DefTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SimPathTab.h
# End Source File
# End Group
# Begin Group "UpdateCheck_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\UpdateCheck\UpdateCheck.h
# End Source File
# End Group
# Begin Group "ThreadClass_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\IL2_UDPThread.h
# End Source File
# Begin Source File

SOURCE=.\InputThread.h
# End Source File
# Begin Source File

SOURCE=.\KeyCheckThread.h
# End Source File
# Begin Source File

SOURCE=.\LFS_UDPThread.h
# End Source File
# Begin Source File

SOURCE=.\OutputThread.h
# End Source File
# Begin Source File

SOURCE=.\SendKeysThread.h
# End Source File
# Begin Source File

SOURCE=.\SimConnectThread.h
# End Source File
# Begin Source File

SOURCE=.\ThreadClass\Thread.h
# End Source File
# Begin Source File

SOURCE=.\TreeThread.h
# End Source File
# End Group
# Begin Group "GridCtrl_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\GridCtrl_src\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl_src\TitleTip.h
# End Source File
# End Group
# Begin Group "NewCellTypes_H"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellDateTime.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellNumeric.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridCellRadio.h
# End Source File
# Begin Source File

SOURCE=.\NewCellTypes\GridURLCell.h
# End Source File
# End Group
# Begin Group "Version_H"

# PROP Default_Filter "H"
# Begin Source File

SOURCE=.\Version\Version.h
# End Source File
# End Group
# Begin Group "Inputs_H"

# PROP Default_Filter "H"
# Begin Source File

SOURCE=.\InputFlagTab.h
# End Source File
# Begin Source File

SOURCE=.\InputsTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NotesPage.h
# End Source File
# Begin Source File

SOURCE=.\NotesTab.h
# End Source File
# Begin Source File

SOURCE=.\SendKeysPage.h
# End Source File
# Begin Source File

SOURCE=.\SendKeysTab.h
# End Source File
# Begin Source File

SOURCE=.\VariablesPage.h
# End Source File
# Begin Source File

SOURCE=.\VariablesTab.h
# End Source File
# End Group
# Begin Group "ResizableLib_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ResizableLib\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableGrip.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableLayout.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableMinMax.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLib\ResizableState.h
# End Source File
# End Group
# Begin Group "w32process_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\w32process\w32process.h
# End Source File
# End Group
# Begin Group "TrayDialog_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TrayDialog\TrayDialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AFflightData.h
# End Source File
# Begin Source File

SOURCE=.\hid_dll\BIAPI.H
# End Source File
# Begin Source File

SOURCE=.\hid_dll\bicfg.h
# End Source File
# Begin Source File

SOURCE=.\hid_dll\biusb.h

!IF  "$(CFG)" == "USIM - Win32 Release"

!ELSEIF  "$(CFG)" == "USIM - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hid_dll\biusb_dev.h

!IF  "$(CFG)" == "USIM - Win32 Release"

!ELSEIF  "$(CFG)" == "USIM - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BMSflightData.h
# End Source File
# Begin Source File

SOURCE=.\COptionTree\CommonRes.h
# End Source File
# Begin Source File

SOURCE=.\F4DataOut.h
# End Source File
# Begin Source File

SOURCE=.\F4FlightData.h
# End Source File
# Begin Source File

SOURCE=.\F4SharedMem.h
# End Source File
# Begin Source File

SOURCE=.\F4VarList.h
# End Source File
# Begin Source File

SOURCE=.\GPXdatarefs.h
# End Source File
# Begin Source File

SOURCE=.\GPXSharedMem.h
# End Source File
# Begin Source File

SOURCE=.\GPXSimData.h
# End Source File
# Begin Source File

SOURCE=.\IL2datarefs.h
# End Source File
# Begin Source File

SOURCE=.\IL2FlightData.h
# End Source File
# Begin Source File

SOURCE=.\hid_dll\intel_hex.h
# End Source File
# Begin Source File

SOURCE=.\KeyboardHook.h
# End Source File
# Begin Source File

SOURCE=.\LFSdatarefs.h
# End Source File
# Begin Source File

SOURCE=.\LFSSimData.h
# End Source File
# Begin Source File

SOURCE=.\MyStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceDevice.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "USIM - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=USIM
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "USIM - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=USIM
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RFdatarefs.h
# End Source File
# Begin Source File

SOURCE=.\RFSharedMem.h
# End Source File
# Begin Source File

SOURCE=.\RFSimData.h
# End Source File
# Begin Source File

SOURCE=.\SaveDevice.h
# End Source File
# Begin Source File

SOURCE=.\SaveUserVars.h
# End Source File
# Begin Source File

SOURCE=.\SimBindatarefs.h
# End Source File
# Begin Source File

SOURCE=.\SimBinSharedMem.h
# End Source File
# Begin Source File

SOURCE=.\SimBinSimData.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TreeDevices.h
# End Source File
# Begin Source File

SOURCE=.\USIM.h
# End Source File
# Begin Source File

SOURCE=.\USIMDlg.h
# End Source File
# Begin Source File

SOURCE=.\VarListDlg.h
# End Source File
# Begin Source File

SOURCE=.\XPdatarefs.h
# End Source File
# Begin Source File

SOURCE=.\XPFlightData.h
# End Source File
# Begin Source File

SOURCE=.\XPSharedMem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\beta_logo_310.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BI_CFG.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\card.ico
# End Source File
# Begin Source File

SOURCE=.\res\card1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\clcd1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\cond.ico
# End Source File
# Begin Source File

SOURCE=.\RES\cond1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\cond_p1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dac1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dac2.ico
# End Source File
# Begin Source File

SOURCE=.\res\dac_dis1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dac_p1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dacs1.ico
# End Source File
# Begin Source File

SOURCE=.\res\devices1.ico
# End Source File
# Begin Source File

SOURCE=.\res\digital.ico
# End Source File
# Begin Source File

SOURCE=.\res\dis_off.ico
# End Source File
# Begin Source File

SOURCE=.\res\dis_off1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dis_off_p.ico
# End Source File
# Begin Source File

SOURCE=.\res\dis_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\dis_p.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dm.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dm_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dm_p.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dm_p1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dmdisp.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dmdisp1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dmdisp_d.ico
# End Source File
# Begin Source File

SOURCE=.\RES\dmxs_p.ico
# End Source File
# Begin Source File

SOURCE=.\res\down.ico
# End Source File
# Begin Source File

SOURCE=".\res\F4-BMS.ico"
# End Source File
# Begin Source File

SOURCE=.\res\f4on1.ico
# End Source File
# Begin Source File

SOURCE=.\res\falcon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\favicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\glcd1.ico
# End Source File
# Begin Source File

SOURCE=.\res\glcd1_p.ico
# End Source File
# Begin Source File

SOURCE=.\res\glcd_dis.ico
# End Source File
# Begin Source File

SOURCE=.\res\gr_off.ico
# End Source File
# Begin Source File

SOURCE=.\res\gr_off_p.ico
# End Source File
# Begin Source File

SOURCE=.\res\gr_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\gr_on_p.ico
# End Source File
# Begin Source File

SOURCE=.\res\help.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00018.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00019.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00020.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00021.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00022.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00023.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00024.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00025.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00026.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00027.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00028.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\input.ico
# End Source File
# Begin Source File

SOURCE=.\res\input1.ico
# End Source File
# Begin Source File

SOURCE=.\res\joystick1.ico
# End Source File
# Begin Source File

SOURCE=.\res\joystick2.ico
# End Source File
# Begin Source File

SOURCE=.\res\jp1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\kbs1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\kbs2.ico
# End Source File
# Begin Source File

SOURCE=.\RES\kbs3.ico
# End Source File
# Begin Source File

SOURCE=.\RES\KEYBRD02.ICO
# End Source File
# Begin Source File

SOURCE=.\RES\KEYBRD03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\keys.ico
# End Source File
# Begin Source File

SOURCE=.\RES\latched.ico
# End Source File
# Begin Source File

SOURCE=.\RES\latched_.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lcd_dis_.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lcd_on.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lcd_on1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lcd_on_p.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lcd_page.ico
# End Source File
# Begin Source File

SOURCE=.\RES\lo1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\minus1.ico
# End Source File
# Begin Source File

SOURCE=.\res\mux1.ico
# End Source File
# Begin Source File

SOURCE=.\res\mux_dis1.ico
# End Source File
# Begin Source File

SOURCE=.\res\NETHOOD.ICO
# End Source File
# Begin Source File

SOURCE=.\res\no_card.ico
# End Source File
# Begin Source File

SOURCE=.\res\non.ico
# End Source File
# Begin Source File

SOURCE=.\RES\notes1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\notes_p1.ico
# End Source File
# Begin Source File

SOURCE=.\res\off.ico
# End Source File
# Begin Source File

SOURCE=.\RES\off1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\offsets1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\offsets_.ico
# End Source File
# Begin Source File

SOURCE=.\res\on.ico
# End Source File
# Begin Source File

SOURCE=.\res\on_red.ico
# End Source File
# Begin Source File

SOURCE=.\res\output1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pc3.ico
# End Source File
# Begin Source File

SOURCE=.\res\plcc.ico
# End Source File
# Begin Source File

SOURCE=.\res\plcc1.ico
# End Source File
# Begin Source File

SOURCE=.\res\plcc_dis.ico
# End Source File
# Begin Source File

SOURCE=.\res\plus1.ico
# End Source File
# Begin Source File

SOURCE=.\res\port1.ico
# End Source File
# Begin Source File

SOURCE=.\res\port3.ico
# End Source File
# Begin Source File

SOURCE=.\res\que1.ico
# End Source File
# Begin Source File

SOURCE=.\res\red_off.ico
# End Source File
# Begin Source File

SOURCE=.\res\red_off1.ico
# End Source File
# Begin Source File

SOURCE=.\res\red_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\red_on1.ico
# End Source File
# Begin Source File

SOURCE=.\res\rotary1.ico
# End Source File
# Begin Source File

SOURCE=.\res\rwr1.ico
# End Source File
# Begin Source File

SOURCE=.\res\rwr_dis1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spi.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spi1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spi_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spi_dis_.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spi_p.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spim1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\spis_p1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\Splitter.cur
# End Source File
# Begin Source File

SOURCE=.\RES\step.ico
# End Source File
# Begin Source File

SOURCE=.\RES\step_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\step_p.ico
# End Source File
# Begin Source File

SOURCE=.\RES\steps.ico
# End Source File
# Begin Source File

SOURCE=.\RES\steps_di.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\update.ico
# End Source File
# Begin Source File

SOURCE=.\res\usb1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Usb2.ico
# End Source File
# Begin Source File

SOURCE=.\res\USIM.ico
# End Source File
# Begin Source File

SOURCE=.\USIM.rc
# End Source File
# Begin Source File

SOURCE=.\res\USIM.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\usim_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\usim_off.ico
# End Source File
# Begin Source File

SOURCE=.\RES\usim_on2.ico
# End Source File
# Begin Source File

SOURCE=.\RES\vars.ico
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxDlg.rtf
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
