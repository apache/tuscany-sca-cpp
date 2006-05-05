# Microsoft Developer Studio Project File - Name="sdo_runtime" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=sdo_runtime - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdo_runtime.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdo_runtime.mak" CFG="sdo_runtime - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdo_runtime - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sdo_runtime - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sdo_runtime - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\runtime\core\Release"
# PROP Intermediate_Dir "..\..\..\runtime\core\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_RUNTIME_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\runtime\core\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /I "$(ZLIB_HOME)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libxml2.lib /nologo /dll /machine:I386 /out:"..\..\..\runtime\core\Release\tuscany_sdo.dll" /libpath:"$(LIBXML2_HOME)\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copyout
PostBuild_Cmds=copy ..\..\..\runtime\core\Release\tuscany_sdo.dll ..\..\..\bin	copy ..\..\..\runtime\core\Release\tuscany_sdo.lib ..\..\..\lib	copy ..\..\..\runtime\core\src\commonj\sdo\Property.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\ChangeSummary.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\CopyHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataFactory.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataGraph.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObject.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObjectInstance.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObjectList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DefaultLogWriter.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\EqualityHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\export.h ..\..\..\include\commonj\sdo	copy\
                 ..\..\..\runtime\core\src\commonj\sdo\HelperProvider.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Logger.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Logging.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\LogWriter.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\PropertyList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\RefCountingPointer.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDOCheck.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDODate.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDORuntime.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDORuntimeException.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDOUtils.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Sequence.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Setting.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SettingList.h ..\..\..\include\commonj\sdo	copy\
                 ..\..\..\runtime\core\src\commonj\sdo\Type.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XMLDocument.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XMLHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XSDHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDO.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\RefCountingObject.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DASValue.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XpathHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeImpl.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeDefinitions.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.h ..\..\..\include\commonj\sdo
# End Special Build Tool

!ELSEIF  "$(CFG)" == "sdo_runtime - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\runtime\core\Debug"
# PROP Intermediate_Dir "..\..\..\runtime\core\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_RUNTIME_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\runtime\core\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /I "$(ZLIB_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_EXPORTS" /YX /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libxml2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\runtime\core\Debug\tuscany_sdo.dll" /pdbtype:sept /libpath:"$(LIBXML2_HOME)\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=copyout
PostBuild_Cmds=copy ..\..\..\runtime\core\Debug\tuscany_sdo.dll ..\..\..\bin	copy ..\..\..\runtime\core\Debug\tuscany_sdo.lib ..\..\..\lib	copy ..\..\..\runtime\core\src\commonj\sdo\Property.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\ChangeSummary.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\CopyHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataFactory.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataGraph.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObject.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObjectInstance.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DataObjectList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DefaultLogWriter.h  ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\EqualityHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\export.h ..\..\..\include\commonj\sdo	copy\
                ..\..\..\runtime\core\src\commonj\sdo\HelperProvider.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Logger.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Logging.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\LogWriter.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\PropertyList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\RefCountingPointer.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDOCheck.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDODate.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDORuntime.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDORuntimeException.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDOUtils.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Sequence.h  ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\Setting.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SettingList.h ..\..\..\include\commonj\sdo	copy\
                ..\..\..\runtime\core\src\commonj\sdo\Type.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeList.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XMLDocument.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XMLHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XSDHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\SDO.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\RefCountingObject.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\DASValue.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\XpathHelper.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeImpl.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\Debug\tuscany_sdo.pdb ..\..\..\bin	copy ..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.h ..\..\..\include\commonj\sdo	copy ..\..\..\runtime\core\src\commonj\sdo\TypeDefinitions.h ..\..\..\include\commonj\sdo	copy\
    ..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.h ..\..\..\include\commonj\sd	copy ..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.h ..\..\..\include\commonj\sdo
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "sdo_runtime - Win32 Release"
# Name "sdo_runtime - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\CopyHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValues.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactoryImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraphImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DefaultLogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\EqualityHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\HelperProvider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Logger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ParserErrorSetter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Property.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinitionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertySetting.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingPointer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attributes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Namespaces.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SchemaInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDODate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoRuntime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDORuntimeException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSAX2Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSchemaSAX2Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLBufferWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLStreamWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDBufferWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDStreamWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Sequence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SequenceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Setting.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SettingList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Type.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionsImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocumentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelperImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLQName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XpathHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelperImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDPropertyInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDTypeInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectListImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummary.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryBuilder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\CopyHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValues.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactoryImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraphImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectInstance.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectListImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\DefaultLogWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\disable_warn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\EqualityHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\export.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\HelperProvider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Logger.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Logging.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\ParserErrorSetter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Property.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinitionImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertySetting.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingPointer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attributes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Namespaces.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SchemaInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoCheck.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDODate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoRuntime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDORuntimeException.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSAX2Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSchemaSAX2Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSPI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLBufferWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLFileWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLStreamWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDBufferWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDFileWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDStreamWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDWriter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Sequence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SequenceImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Setting.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\SettingList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\Type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionsImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocumentImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelperImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLQName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XpathHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelperImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDPropertyInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDTypeInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE="..\..\..\Committers Guide.txt"
# End Source File
# Begin Source File

SOURCE=..\..\..\readme.txt
# End Source File
# End Target
# End Project
