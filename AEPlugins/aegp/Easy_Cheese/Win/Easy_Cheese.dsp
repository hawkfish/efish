# Microsoft Developer Studio Project File - Name="Easy_Cheese" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Easy_Cheese - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Easy_Cheese.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Easy_Cheese.mak" CFG="Easy_Cheese - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Easy_Cheese - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Easy_Cheese"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "..\..\..\..\adm" /I "..\..\..\..\PhotoshopAPI\ADM" /I "..\\" /I "..\..\..\Headers" /I "..\..\..\Util" /I "..\..\..\Headers\PICA" /I "..\..\..\Headers\ADM" /I "..\..\..\Headers\AS" /I "..\..\..\..\eSellerate SDK\eSellerate Static Library" /I "..\..\..\..\eSellerate SDK\Validate Static Library" /D "MSWindows" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FAs /FR /YX"AE_Effect.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib validateLibrary.lib eSellerateLibrary.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\Built_Plug-ins\Win\AEGP\AEGP\Color Pal.aex" /libpath:"..\..\..\..\eSellerate SDK\Validate Static Library" /libpath:"..\..\..\..\eSellerate SDK\eSellerate Static Library"
# SUBTRACT LINK32 /verbose /pdb:none /incremental:no /map
# Begin Target

# Name "Easy_Cheese - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "PICA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPAccess.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPAdapts.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPCaches.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPErrorCodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPFiles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPMData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPPiPL.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPPlugs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPProps.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPStrngs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPSuites.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPTypes.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE="D:\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# End Group
# Begin Group "AE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Headers\A.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_AdvEffectSuites.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_Effect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_EffectCB.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_EffectCBSuites.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_EffectSuites.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_EffectUI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_GeneralPlug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_Hook.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_IO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AE_Macros.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\AEGP_SuiteHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\entry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\FIEL_Public.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PF_Masks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PR_Public.h
# End Source File
# End Group
# Begin Group "ADM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ADM_Constants.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADM_PaletteUIConstants.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMDialogGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\Legacy\ADMDialogGroupOld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\Legacy\ADMDialogOld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMEntry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\Legacy\ADMEntryOld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\Legacy\ADMItemOld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\Legacy\ADMListOld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMNotifier.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMTracker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\ADMTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AS\ASConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AS\ASHelp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AS\ASPragma.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\AS\ASTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\DlgLayoutUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\PICA\SPInterf.h
# End Source File
# End Group
# Begin Group "adm Headers"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\..\..\adm\adm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_button.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_callback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_color.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_deck.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_dialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_drawer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_edit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_entry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_fontdialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_group.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_item.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_label.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_menu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_persist.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_slider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_suites.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_tracker.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_treeview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_user.h
# End Source File
# Begin Source File

SOURCE=..\ChipList.h
# End Source File
# Begin Source File

SOURCE=..\ColorPal.h
# End Source File
# Begin Source File

SOURCE=..\ColorPalette.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\eSellerate SDK\eSellerate Static Library\esellerate.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\ESellerateEngine.h
# End Source File
# Begin Source File

SOURCE=..\HotChip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\PurchaseDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\RegistrationDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\RegistrationStorage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\SerialNumber.h
# End Source File
# Begin Source File

SOURCE=..\TabbedPalette.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\eSellerate SDK\Validate Static Library\validate.h"
# End Source File
# End Group
# Begin Source File

SOURCE=..\Easy_Cheese.h
# End Source File
# Begin Source File

SOURCE=..\Easy_Cheese_Strings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Headers\ADM\SDK_ADM.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\ColorPal.rc
# End Source File
# Begin Source File

SOURCE=..\Easy_Cheese_PiPL.r
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Compiling the PiPL
IntDir=.\Debug
ProjDir=.
InputPath=..\Easy_Cheese_PiPL.r
InputName=Easy_Cheese_PiPL

"$(ProjDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /D "MSWindows" /EP ..\$(InputName).r > $(IntDir)\$(InputName).rr 
	$(ProjDir)\..\..\..\Resources\PiPLTool $(IntDir)\$(InputName).rr  $(IntDir)\$(InputName).rrc 
	cl /D "MSWindows" /EP $(IntDir)\$(InputName).rrc > $(ProjDir)\$(InputName).rc 
	
# End Custom Build
# End Source File
# Begin Source File

SOURCE=.\Easy_Cheese_PiPL.rc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Supporting Code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Util\AEGP_SuiteHandler.cpp
# End Source File
# End Group
# Begin Group "adm Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\adm\adm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_button.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_callback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_color.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_deck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_drawer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_edit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_entry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_fontdialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_group.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_item.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_label.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_list.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_listview.cpp
# ADD CPP /GR /GX
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_menu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_persist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_slider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_suites.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_tracker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\adm\adm_user.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\ChipList.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorPal.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorPalESellerate.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorPalESellerateWin.cpp
# End Source File
# Begin Source File

SOURCE=..\ColorPalette.cpp
# End Source File
# Begin Source File

SOURCE=..\Easy_Cheese.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\ESellerateEngine.cpp
# SUBTRACT CPP /X
# End Source File
# Begin Source File

SOURCE=..\..\..\..\eSellerateWin\eSellerateEngine.dll
# End Source File
# Begin Source File

SOURCE=..\HotChip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\PurchaseDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\RegistrationDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\RegistrationStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Util\SerialNumber.c
# End Source File
# Begin Source File

SOURCE=..\TabbedPalette.cpp
# End Source File
# End Target
# End Project
