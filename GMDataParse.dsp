# Microsoft Developer Studio Project File - Name="GMDataParse" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GMDataParse - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GMDataParse.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GMDataParse.mak" CFG="GMDataParse - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GMDataParse - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GMDataParse - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GMDataParse - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GMDataParse - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GMDataParse - Win32 Release"
# Name "GMDataParse - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirectoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GMDataParse.cpp
# End Source File
# Begin Source File

SOURCE=.\GMDataParse.rc
# End Source File
# Begin Source File

SOURCE=.\GMDataParseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PraseEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocol02_101Frame.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinLib.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinLib\skinradio.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DirectoryDlg.h
# End Source File
# Begin Source File

SOURCE=.\GMDataParse.h
# End Source File
# Begin Source File

SOURCE=.\GMDataParseDlg.h
# End Source File
# Begin Source File

SOURCE=.\PraseEdit.h
# End Source File
# Begin Source File

SOURCE=.\Protocol.h
# End Source File
# Begin Source File

SOURCE=.\Protocol101.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinButton.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinDialog.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinEdit.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\SkinLib.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\skinradio.h
# End Source File
# Begin Source File

SOURCE=.\SkinLib\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GMDataParse.ico
# End Source File
# Begin Source File

SOURCE=.\res\GMDataParse.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
