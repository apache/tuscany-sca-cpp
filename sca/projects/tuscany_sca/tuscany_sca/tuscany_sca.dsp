# Microsoft Developer Studio Project File - Name="tuscany_sca" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=tuscany_sca - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca.mak" CFG="tuscany_sca - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tuscany_sca - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCA_EXPORTS" /FD /c
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
# ADD LINK32 axis2_engine.lib tuscany_sdo_axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /dll /machine:I386 /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\..\runtime\core\deploy.bat ..\..\..\ Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "tuscany_sca - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCA_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 axis2_engine.lib tuscany_sdo_axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=..\..\..\runtime\core\deploy.bat ..\..\..\ Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "tuscany_sca - Win32 Release"
# Name "tuscany_sca - Win32 Debug"
# Begin Group "tuscany/sca"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\Operation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\Operation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\SCARuntime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\SCARuntime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceProxy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceWrapper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\TuscanyRuntime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\TuscanyRuntime.h
# End Source File
# End Group
# Begin Group "model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Binding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Binding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Component.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Component.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ComponentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ComponentType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Composite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Composite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeReference.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeService.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeService.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Contract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Contract.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Interface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ModelLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ModelLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Reference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Reference.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Service.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Service.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Wire.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Wire.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLOperation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WsdlOperation.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\DefaultLogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\DefaultLogWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Exceptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Exceptions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\File.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\File.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\FileLogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\FileLogWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Library.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Library.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Logger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Logger.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Logging.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Utils.h
# End Source File
# End Group
# Begin Group "extension"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ImplementationExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ImplementationExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\InterfaceExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\InterfaceExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ReferenceBindingExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ReferenceBindingExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ServiceBindingExtension.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ServiceBindingExtension.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\runtime\core\src\tuscany\sca\export.h
# End Source File
# End Group
# Begin Group "xsd"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\xsd\sca-core.xsd"
# End Source File
# Begin Source File

SOURCE="..\..\..\xsd\sca-implementation-composite.xsd"
# End Source File
# Begin Source File

SOURCE="..\..\..\xsd\sca-implementation-java.xsd"
# End Source File
# Begin Source File

SOURCE="..\..\..\xsd\sca-interface-java.xsd"
# End Source File
# Begin Source File

SOURCE="..\..\..\xsd\sca-interface-wsdl.xsd"
# End Source File
# Begin Source File

SOURCE=..\..\..\xsd\sca.xsd
# End Source File
# Begin Source File

SOURCE=..\..\..\xsd\tuscany.xsd
# End Source File
# End Group
# End Target
# End Project
