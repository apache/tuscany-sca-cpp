# Microsoft Developer Studio Project File - Name="tuscany_sca_cpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tuscany_sca_cpp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_cpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_cpp.mak" CFG="tuscany_sca_cpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_cpp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_cpp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tuscany_sca_cpp - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/cpp/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 tuscany_sca.lib tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\tuscany_sca_cpp.dll ..\..\..\deploy\extensions\cpp\bin	copy Release\tuscany_sca_cpp.lib ..\..\..\deploy\extensions\cpp\lib	copy ..\..\..\runtime\extensions\cpp\xsd\*.xsd ..\..\..\deploy\extensions\cpp\xsd	copy ..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\*.h ..\..\..\deploy\extensions\cpp\include\tuscany\sca\cpp	copy ..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\*.h ..\..\..\deploy\extensions\cpp\include\tuscany\sca\cpp\model	copy ..\..\..\runtime\extensions\cpp\src\osoa\sca\*.h ..\..\..\deploy\extensions\cpp\include\osoa\sca
# End Special Build Tool

!ELSEIF  "$(CFG)" == "tuscany_sca_cpp - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/cpp/src" /I "../../../deploy/include" /I "$(TUSCANY_SDOCPP)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_CPP_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 tuscany_sca.lib tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\tuscany_sca_cpp.dll ..\..\..\deploy\extensions\cpp\bin	copy Debug\tuscany_sca_cpp.pdb ..\..\..\deploy\extensions\cpp\bin	copy Debug\tuscany_sca_cpp.lib ..\..\..\deploy\extensions\cpp\lib	copy ..\..\..\runtime\extensions\cpp\xsd\*.xsd ..\..\..\deploy\extensions\cpp\xsd	copy ..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\*.h ..\..\..\deploy\extensions\cpp\include\tuscany\sca\cpp	copy ..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\*.h ..\..\..\deploy\extensions\cpp\include\tuscany\sca\cpp\model	copy ..\..\..\runtime\extensions\cpp\src\osoa\sca\*.h ..\..\..\deploy\extensions\cpp\include\osoa\sca
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "tuscany_sca_cpp - Win32 Release"
# Name "tuscany_sca_cpp - Win32 Debug"
# Begin Group "xsd"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\runtime\extensions\cpp\xsd\sca-implementation-cpp.xsd"
# End Source File
# Begin Source File

SOURCE="..\..\..\runtime\extensions\cpp\xsd\sca-interface-cpp.xsd"
# End Source File
# End Group
# Begin Group "osoa/sca"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ComponentContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ComponentContext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\CompositeContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\CompositeContext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\export.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\sca.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ServiceList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ServiceList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\osoa\sca\ServiceRuntimeException.h
# End Source File
# End Group
# Begin Group "tuscany/sca/cpp"

# PROP Default_Filter ""
# Begin Group "model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPImplementation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPReferenceBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPReferenceBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPServiceBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\model\CPPServiceBinding.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\ComponentContextImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\ComponentContextImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CompositeContextImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CompositeContextImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPImplementationExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPImplementationExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPInterfaceExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPInterfaceExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\extensions\cpp\src\tuscany\sca\cpp\CPPServiceWrapper.h
# End Source File
# End Group
# End Target
# End Project
