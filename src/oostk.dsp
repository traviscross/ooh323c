# Microsoft Developer Studio Project File - Name="oostk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=oostk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "oostk.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "oostk.mak" CFG="oostk - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "oostk - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "oostk - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "oostk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Og /Os /Oy /Gy /I "." /I "..\media" /I ".\h323" /D "NDEBUG" /D "_OOINFO" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WIN32" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\release\oostk.lib"

!ELSEIF  "$(CFG)" == "oostk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /Od /I "." /I "..\media" /I ".\h323" /D "_DEBUG" /D "_OODEBUGC" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WIN32" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\debug\oostk.lib"

!ENDIF

# Begin Target

# Name "oostk - Win32 Release"
# Name "oostk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\context.c
# End Source File
# Begin Source File

SOURCE=.\decode.c
# End Source File
# Begin Source File

SOURCE=.\dlist.c
# End Source File
# Begin Source File

SOURCE=.\encode.c
# End Source File
# Begin Source File

SOURCE=.\errmgmt.c
# End Source File
# Begin Source File

SOURCE=.\eventHandler.c
# End Source File
# Begin Source File

SOURCE=".\h323\H235-SECURITY-MESSAGESDec.c"
# End Source File
# Begin Source File

SOURCE=".\h323\H235-SECURITY-MESSAGESEnc.c"
# End Source File
# Begin Source File

SOURCE=".\h323\H323-MESSAGES.c"
# End Source File
# Begin Source File

SOURCE=".\h323\H323-MESSAGESDec.c"
# End Source File
# Begin Source File

SOURCE=".\h323\H323-MESSAGESEnc.c"
# End Source File
# Begin Source File

SOURCE=.\memheap.c
# End Source File
# Begin Source File

SOURCE=".\h323\MULTIMEDIA-SYSTEM-CONTROL.c"
# End Source File
# Begin Source File

SOURCE=".\h323\MULTIMEDIA-SYSTEM-CONTROLDec.c"
# End Source File
# Begin Source File

SOURCE=".\h323\MULTIMEDIA-SYSTEM-CONTROLEnc.c"
# End Source File
# Begin Source File

SOURCE=.\ooCalls.c
# End Source File
# Begin Source File

SOURCE=.\ooCapability.c
# End Source File
# Begin Source File

SOURCE=.\oochannels.c
# End Source File
# Begin Source File

SOURCE=.\ooDateTime.c
# End Source File
# Begin Source File

SOURCE=.\ooGkClient.c
# End Source File
# Begin Source File

SOURCE=.\ooh245.c
# End Source File
# Begin Source File

SOURCE=.\ooh323.c
# End Source File
# Begin Source File

SOURCE=.\ooh323ep.c
# End Source File
# Begin Source File

SOURCE=.\ooLogChan.c
# End Source File
# Begin Source File

SOURCE=.\ooports.c
# End Source File
# Begin Source File

SOURCE=.\ooq931.c
# End Source File
# Begin Source File

SOURCE=.\ooSocket.c
# End Source File
# Begin Source File

SOURCE=.\ooStackCmds.c
# End Source File
# Begin Source File

SOURCE=.\ooTimer.c
# End Source File
# Begin Source File

SOURCE=.\ootrace.c
# End Source File
# Begin Source File

SOURCE=.\ooUtils.c
# End Source File
# Begin Source File

SOURCE=.\perutil.c
# End Source File
# Begin Source File

SOURCE=.\printHandler.c
# End Source File
# Begin Source File

SOURCE=.\rtctype.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\eventHandler.h
# End Source File
# Begin Source File

SOURCE=".\h323\H235-SECURITY-MESSAGES.h"
# End Source File
# Begin Source File

SOURCE=".\h323\H323-MESSAGES.h"
# End Source File
# Begin Source File

SOURCE=.\memheap.h
# End Source File
# Begin Source File

SOURCE=".\h323\MULTIMEDIA-SYSTEM-CONTROL.h"
# End Source File
# Begin Source File

SOURCE=.\ooasn1.h
# End Source File
# Begin Source File

SOURCE=.\ooCalls.h
# End Source File
# Begin Source File

SOURCE=.\ooCapability.h
# End Source File
# Begin Source File

SOURCE=.\oochannels.h
# End Source File
# Begin Source File

SOURCE=.\ooCommon.h
# End Source File
# Begin Source File

SOURCE=.\ooDateTime.h
# End Source File
# Begin Source File

SOURCE=.\ooGkClient.h
# End Source File
# Begin Source File

SOURCE=.\ooh245.h
# End Source File
# Begin Source File

SOURCE=.\ooh323.h
# End Source File
# Begin Source File

SOURCE=.\ooh323ep.h
# End Source File
# Begin Source File

SOURCE=.\oohdr.h
# End Source File
# Begin Source File

SOURCE=.\ooLogChan.h
# End Source File
# Begin Source File

SOURCE=.\ooper.h
# End Source File
# Begin Source File

SOURCE=.\ooports.h
# End Source File
# Begin Source File

SOURCE=.\ooq931.h
# End Source File
# Begin Source File

SOURCE=.\ooSocket.h
# End Source File
# Begin Source File

SOURCE=.\ooStackCmds.h
# End Source File
# Begin Source File

SOURCE=.\ooTimer.h
# End Source File
# Begin Source File

SOURCE=.\ootrace.h
# End Source File
# Begin Source File

SOURCE=.\ootypes.h
# End Source File
# Begin Source File

SOURCE=.\ooUtils.h
# End Source File
# Begin Source File

SOURCE=.\printHandler.h
# End Source File
# Begin Source File

SOURCE=.\rtctype.h
# End Source File
# Begin Source File

SOURCE=.\SList.h
# End Source File
# End Group
# End Target
# End Project
