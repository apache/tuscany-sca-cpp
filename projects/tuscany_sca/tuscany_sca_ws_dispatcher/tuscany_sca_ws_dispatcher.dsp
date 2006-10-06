# Microsoft Developer Studio Project File - Name="tuscany_sca_ws_dispatcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tuscany_sca_ws_dispatcher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_ws_dispatcher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_ws_dispatcher.mak" CFG="tuscany_sca_ws_dispatcher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_ws_dispatcher - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_ws_dispatcher - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_ws_dispatcher_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_DISPATCHER_EXPORTS" /FD /c
# SUBTRACT CPP /u /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib /nologo /dll /machine:I386 /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\..\runtime\extensions\ws\service\deploymodule.bat ..\..\..\ Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_ws_dispatcher_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_DISPATCHER_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /u /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(AXIS2C_HOME)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\..\runtime\extensions\ws\service\deploymodule.bat ..\..\..\ Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "tuscany_sca_ws_dispatcher - Win32 Release"
# Name "tuscany_sca_ws_dispatcher - Win32 Debug"
# Begin Group "tuscany/sca/ws"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2DispatcherModule.cpp
# End Source File
# End Group
# Begin Group "xsd"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\runtime\extensions\ws\xsd\sca-binding-webservice.xsd"
# End Source File
# End Group
# End Target
# End Project
