# Microsoft Developer Studio Generated NMAKE File, Based on sdo_runtime.dsp
!IF "$(CFG)" == ""
CFG=sdo_runtime - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sdo_runtime - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sdo_runtime - Win32 Release" && "$(CFG)" != "sdo_runtime - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sdo_runtime - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sdo.dll"


CLEAN :
	-@erase "$(INTDIR)\ChangedDataObjectListImpl.obj"
	-@erase "$(INTDIR)\ChangeSummary.obj"
	-@erase "$(INTDIR)\ChangeSummaryBuilder.obj"
	-@erase "$(INTDIR)\ChangeSummaryImpl.obj"
	-@erase "$(INTDIR)\CopyHelper.obj"
	-@erase "$(INTDIR)\DASProperty.obj"
	-@erase "$(INTDIR)\DASType.obj"
	-@erase "$(INTDIR)\DASValue.obj"
	-@erase "$(INTDIR)\DASValues.obj"
	-@erase "$(INTDIR)\DataFactory.obj"
	-@erase "$(INTDIR)\DataFactoryImpl.obj"
	-@erase "$(INTDIR)\DataGraph.obj"
	-@erase "$(INTDIR)\DataGraphImpl.obj"
	-@erase "$(INTDIR)\DataObject.obj"
	-@erase "$(INTDIR)\DataObjectImpl.obj"
	-@erase "$(INTDIR)\DataObjectInstance.obj"
	-@erase "$(INTDIR)\DataObjectList.obj"
	-@erase "$(INTDIR)\DataObjectListImpl.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\EqualityHelper.obj"
	-@erase "$(INTDIR)\GroupDefinition.obj"
	-@erase "$(INTDIR)\GroupEvent.obj"
	-@erase "$(INTDIR)\HelperProvider.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ParserErrorSetter.obj"
	-@erase "$(INTDIR)\Property.obj"
	-@erase "$(INTDIR)\PropertyDefinition.obj"
	-@erase "$(INTDIR)\PropertyDefinitionImpl.obj"
	-@erase "$(INTDIR)\PropertyImpl.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertySetting.obj"
	-@erase "$(INTDIR)\RefCountingObject.obj"
	-@erase "$(INTDIR)\RefCountingPointer.obj"
	-@erase "$(INTDIR)\SAX2Attribute.obj"
	-@erase "$(INTDIR)\SAX2Attributes.obj"
	-@erase "$(INTDIR)\SAX2Namespaces.obj"
	-@erase "$(INTDIR)\SAX2Parser.obj"
	-@erase "$(INTDIR)\SchemaInfo.obj"
	-@erase "$(INTDIR)\SdoCheck.obj"
	-@erase "$(INTDIR)\SDODate.obj"
	-@erase "$(INTDIR)\SdoRuntime.obj"
	-@erase "$(INTDIR)\SDORuntimeException.obj"
	-@erase "$(INTDIR)\SDOSAX2Parser.obj"
	-@erase "$(INTDIR)\SDOSchemaSAX2Parser.obj"
	-@erase "$(INTDIR)\SDOUtils.obj"
	-@erase "$(INTDIR)\SDOXMLBufferWriter.obj"
	-@erase "$(INTDIR)\SDOXMLFileWriter.obj"
	-@erase "$(INTDIR)\SDOXMLStreamWriter.obj"
	-@erase "$(INTDIR)\SDOXMLString.obj"
	-@erase "$(INTDIR)\SDOXMLWriter.obj"
	-@erase "$(INTDIR)\SDOXSDBufferWriter.obj"
	-@erase "$(INTDIR)\SDOXSDFileWriter.obj"
	-@erase "$(INTDIR)\SDOXSDStreamWriter.obj"
	-@erase "$(INTDIR)\SDOXSDWriter.obj"
	-@erase "$(INTDIR)\Sequence.obj"
	-@erase "$(INTDIR)\SequenceImpl.obj"
	-@erase "$(INTDIR)\Setting.obj"
	-@erase "$(INTDIR)\SettingList.obj"
	-@erase "$(INTDIR)\Type.obj"
	-@erase "$(INTDIR)\TypeDefinition.obj"
	-@erase "$(INTDIR)\TypeDefinitionImpl.obj"
	-@erase "$(INTDIR)\TypeDefinitions.obj"
	-@erase "$(INTDIR)\TypeDefinitionsImpl.obj"
	-@erase "$(INTDIR)\TypeImpl.obj"
	-@erase "$(INTDIR)\TypeList.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XMLDocument.obj"
	-@erase "$(INTDIR)\XMLDocumentImpl.obj"
	-@erase "$(INTDIR)\XMLHelper.obj"
	-@erase "$(INTDIR)\XMLHelperImpl.obj"
	-@erase "$(INTDIR)\XMLQName.obj"
	-@erase "$(INTDIR)\XpathHelper.obj"
	-@erase "$(INTDIR)\XSDHelper.obj"
	-@erase "$(INTDIR)\XSDHelperImpl.obj"
	-@erase "$(INTDIR)\XSDPropertyInfo.obj"
	-@erase "$(INTDIR)\XSDTypeInfo.obj"
	-@erase "$(OUTDIR)\tuscany_sdo.dll"
	-@erase "$(OUTDIR)\tuscany_sdo.exp"
	-@erase "$(OUTDIR)\tuscany_sdo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\runtime\core\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /I "$(ZLIB_HOME)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_runtime.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libxml2.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sdo.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sdo.dll" /implib:"$(OUTDIR)\tuscany_sdo.lib" /libpath:"$(LIBXML2_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangedDataObjectListImpl.obj" \
	"$(INTDIR)\ChangeSummary.obj" \
	"$(INTDIR)\ChangeSummaryBuilder.obj" \
	"$(INTDIR)\ChangeSummaryImpl.obj" \
	"$(INTDIR)\CopyHelper.obj" \
	"$(INTDIR)\DASProperty.obj" \
	"$(INTDIR)\DASType.obj" \
	"$(INTDIR)\DASValue.obj" \
	"$(INTDIR)\DASValues.obj" \
	"$(INTDIR)\DataFactory.obj" \
	"$(INTDIR)\DataFactoryImpl.obj" \
	"$(INTDIR)\DataGraph.obj" \
	"$(INTDIR)\DataGraphImpl.obj" \
	"$(INTDIR)\DataObject.obj" \
	"$(INTDIR)\DataObjectImpl.obj" \
	"$(INTDIR)\DataObjectInstance.obj" \
	"$(INTDIR)\DataObjectList.obj" \
	"$(INTDIR)\DataObjectListImpl.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\EqualityHelper.obj" \
	"$(INTDIR)\GroupDefinition.obj" \
	"$(INTDIR)\GroupEvent.obj" \
	"$(INTDIR)\HelperProvider.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\ParserErrorSetter.obj" \
	"$(INTDIR)\Property.obj" \
	"$(INTDIR)\PropertyDefinition.obj" \
	"$(INTDIR)\PropertyDefinitionImpl.obj" \
	"$(INTDIR)\PropertyImpl.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\PropertySetting.obj" \
	"$(INTDIR)\RefCountingObject.obj" \
	"$(INTDIR)\RefCountingPointer.obj" \
	"$(INTDIR)\SAX2Attribute.obj" \
	"$(INTDIR)\SAX2Attributes.obj" \
	"$(INTDIR)\SAX2Namespaces.obj" \
	"$(INTDIR)\SAX2Parser.obj" \
	"$(INTDIR)\SchemaInfo.obj" \
	"$(INTDIR)\SdoCheck.obj" \
	"$(INTDIR)\SDODate.obj" \
	"$(INTDIR)\SdoRuntime.obj" \
	"$(INTDIR)\SDORuntimeException.obj" \
	"$(INTDIR)\SDOSAX2Parser.obj" \
	"$(INTDIR)\SDOSchemaSAX2Parser.obj" \
	"$(INTDIR)\SDOUtils.obj" \
	"$(INTDIR)\SDOXMLBufferWriter.obj" \
	"$(INTDIR)\SDOXMLFileWriter.obj" \
	"$(INTDIR)\SDOXMLStreamWriter.obj" \
	"$(INTDIR)\SDOXMLString.obj" \
	"$(INTDIR)\SDOXMLWriter.obj" \
	"$(INTDIR)\SDOXSDBufferWriter.obj" \
	"$(INTDIR)\SDOXSDFileWriter.obj" \
	"$(INTDIR)\SDOXSDStreamWriter.obj" \
	"$(INTDIR)\SDOXSDWriter.obj" \
	"$(INTDIR)\Sequence.obj" \
	"$(INTDIR)\SequenceImpl.obj" \
	"$(INTDIR)\Setting.obj" \
	"$(INTDIR)\SettingList.obj" \
	"$(INTDIR)\Type.obj" \
	"$(INTDIR)\TypeDefinition.obj" \
	"$(INTDIR)\TypeDefinitionImpl.obj" \
	"$(INTDIR)\TypeDefinitions.obj" \
	"$(INTDIR)\TypeDefinitionsImpl.obj" \
	"$(INTDIR)\TypeImpl.obj" \
	"$(INTDIR)\TypeList.obj" \
	"$(INTDIR)\XMLDocument.obj" \
	"$(INTDIR)\XMLDocumentImpl.obj" \
	"$(INTDIR)\XMLHelper.obj" \
	"$(INTDIR)\XMLHelperImpl.obj" \
	"$(INTDIR)\XMLQName.obj" \
	"$(INTDIR)\XpathHelper.obj" \
	"$(INTDIR)\XSDHelper.obj" \
	"$(INTDIR)\XSDHelperImpl.obj" \
	"$(INTDIR)\XSDPropertyInfo.obj" \
	"$(INTDIR)\XSDTypeInfo.obj"

"$(OUTDIR)\tuscany_sdo.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
PostBuild_Desc=copyout
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sdo.dll"
   copy Release\tuscany_sdo.dll ..\..\..\deploy\bin
	copy Release\tuscany_sdo.lib ..\..\..\deploy\lib
	copy ..\..\..\runtime\core\src\commonj\sdo\*.h ..\..\..\deploy\include\commonj\sdo
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "sdo_runtime - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sdo.dll"


CLEAN :
	-@erase "$(INTDIR)\ChangedDataObjectListImpl.obj"
	-@erase "$(INTDIR)\ChangeSummary.obj"
	-@erase "$(INTDIR)\ChangeSummaryBuilder.obj"
	-@erase "$(INTDIR)\ChangeSummaryImpl.obj"
	-@erase "$(INTDIR)\CopyHelper.obj"
	-@erase "$(INTDIR)\DASProperty.obj"
	-@erase "$(INTDIR)\DASType.obj"
	-@erase "$(INTDIR)\DASValue.obj"
	-@erase "$(INTDIR)\DASValues.obj"
	-@erase "$(INTDIR)\DataFactory.obj"
	-@erase "$(INTDIR)\DataFactoryImpl.obj"
	-@erase "$(INTDIR)\DataGraph.obj"
	-@erase "$(INTDIR)\DataGraphImpl.obj"
	-@erase "$(INTDIR)\DataObject.obj"
	-@erase "$(INTDIR)\DataObjectImpl.obj"
	-@erase "$(INTDIR)\DataObjectInstance.obj"
	-@erase "$(INTDIR)\DataObjectList.obj"
	-@erase "$(INTDIR)\DataObjectListImpl.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\EqualityHelper.obj"
	-@erase "$(INTDIR)\GroupDefinition.obj"
	-@erase "$(INTDIR)\GroupEvent.obj"
	-@erase "$(INTDIR)\HelperProvider.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ParserErrorSetter.obj"
	-@erase "$(INTDIR)\Property.obj"
	-@erase "$(INTDIR)\PropertyDefinition.obj"
	-@erase "$(INTDIR)\PropertyDefinitionImpl.obj"
	-@erase "$(INTDIR)\PropertyImpl.obj"
	-@erase "$(INTDIR)\PropertyList.obj"
	-@erase "$(INTDIR)\PropertySetting.obj"
	-@erase "$(INTDIR)\RefCountingObject.obj"
	-@erase "$(INTDIR)\RefCountingPointer.obj"
	-@erase "$(INTDIR)\SAX2Attribute.obj"
	-@erase "$(INTDIR)\SAX2Attributes.obj"
	-@erase "$(INTDIR)\SAX2Namespaces.obj"
	-@erase "$(INTDIR)\SAX2Parser.obj"
	-@erase "$(INTDIR)\SchemaInfo.obj"
	-@erase "$(INTDIR)\SdoCheck.obj"
	-@erase "$(INTDIR)\SDODate.obj"
	-@erase "$(INTDIR)\SdoRuntime.obj"
	-@erase "$(INTDIR)\SDORuntimeException.obj"
	-@erase "$(INTDIR)\SDOSAX2Parser.obj"
	-@erase "$(INTDIR)\SDOSchemaSAX2Parser.obj"
	-@erase "$(INTDIR)\SDOUtils.obj"
	-@erase "$(INTDIR)\SDOXMLBufferWriter.obj"
	-@erase "$(INTDIR)\SDOXMLFileWriter.obj"
	-@erase "$(INTDIR)\SDOXMLStreamWriter.obj"
	-@erase "$(INTDIR)\SDOXMLString.obj"
	-@erase "$(INTDIR)\SDOXMLWriter.obj"
	-@erase "$(INTDIR)\SDOXSDBufferWriter.obj"
	-@erase "$(INTDIR)\SDOXSDFileWriter.obj"
	-@erase "$(INTDIR)\SDOXSDStreamWriter.obj"
	-@erase "$(INTDIR)\SDOXSDWriter.obj"
	-@erase "$(INTDIR)\Sequence.obj"
	-@erase "$(INTDIR)\SequenceImpl.obj"
	-@erase "$(INTDIR)\Setting.obj"
	-@erase "$(INTDIR)\SettingList.obj"
	-@erase "$(INTDIR)\Type.obj"
	-@erase "$(INTDIR)\TypeDefinition.obj"
	-@erase "$(INTDIR)\TypeDefinitionImpl.obj"
	-@erase "$(INTDIR)\TypeDefinitions.obj"
	-@erase "$(INTDIR)\TypeDefinitionsImpl.obj"
	-@erase "$(INTDIR)\TypeImpl.obj"
	-@erase "$(INTDIR)\TypeList.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XMLDocument.obj"
	-@erase "$(INTDIR)\XMLDocumentImpl.obj"
	-@erase "$(INTDIR)\XMLHelper.obj"
	-@erase "$(INTDIR)\XMLHelperImpl.obj"
	-@erase "$(INTDIR)\XMLQName.obj"
	-@erase "$(INTDIR)\XpathHelper.obj"
	-@erase "$(INTDIR)\XSDHelper.obj"
	-@erase "$(INTDIR)\XSDHelperImpl.obj"
	-@erase "$(INTDIR)\XSDPropertyInfo.obj"
	-@erase "$(INTDIR)\XSDTypeInfo.obj"
	-@erase "$(OUTDIR)\tuscany_sdo.dll"
	-@erase "$(OUTDIR)\tuscany_sdo.exp"
	-@erase "$(OUTDIR)\tuscany_sdo.ilk"
	-@erase "$(OUTDIR)\tuscany_sdo.lib"
	-@erase "$(OUTDIR)\tuscany_sdo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\runtime\core\src" /I "$(LIBXML2_HOME)\include" /I "$(ICONV_HOME)\include" /I "$(ZLIB_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_runtime.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libxml2.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sdo.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sdo.dll" /implib:"$(OUTDIR)\tuscany_sdo.lib" /pdbtype:sept /libpath:"$(LIBXML2_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangedDataObjectListImpl.obj" \
	"$(INTDIR)\ChangeSummary.obj" \
	"$(INTDIR)\ChangeSummaryBuilder.obj" \
	"$(INTDIR)\ChangeSummaryImpl.obj" \
	"$(INTDIR)\CopyHelper.obj" \
	"$(INTDIR)\DASProperty.obj" \
	"$(INTDIR)\DASType.obj" \
	"$(INTDIR)\DASValue.obj" \
	"$(INTDIR)\DASValues.obj" \
	"$(INTDIR)\DataFactory.obj" \
	"$(INTDIR)\DataFactoryImpl.obj" \
	"$(INTDIR)\DataGraph.obj" \
	"$(INTDIR)\DataGraphImpl.obj" \
	"$(INTDIR)\DataObject.obj" \
	"$(INTDIR)\DataObjectImpl.obj" \
	"$(INTDIR)\DataObjectInstance.obj" \
	"$(INTDIR)\DataObjectList.obj" \
	"$(INTDIR)\DataObjectListImpl.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\EqualityHelper.obj" \
	"$(INTDIR)\GroupDefinition.obj" \
	"$(INTDIR)\GroupEvent.obj" \
	"$(INTDIR)\HelperProvider.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\ParserErrorSetter.obj" \
	"$(INTDIR)\Property.obj" \
	"$(INTDIR)\PropertyDefinition.obj" \
	"$(INTDIR)\PropertyDefinitionImpl.obj" \
	"$(INTDIR)\PropertyImpl.obj" \
	"$(INTDIR)\PropertyList.obj" \
	"$(INTDIR)\PropertySetting.obj" \
	"$(INTDIR)\RefCountingObject.obj" \
	"$(INTDIR)\RefCountingPointer.obj" \
	"$(INTDIR)\SAX2Attribute.obj" \
	"$(INTDIR)\SAX2Attributes.obj" \
	"$(INTDIR)\SAX2Namespaces.obj" \
	"$(INTDIR)\SAX2Parser.obj" \
	"$(INTDIR)\SchemaInfo.obj" \
	"$(INTDIR)\SdoCheck.obj" \
	"$(INTDIR)\SDODate.obj" \
	"$(INTDIR)\SdoRuntime.obj" \
	"$(INTDIR)\SDORuntimeException.obj" \
	"$(INTDIR)\SDOSAX2Parser.obj" \
	"$(INTDIR)\SDOSchemaSAX2Parser.obj" \
	"$(INTDIR)\SDOUtils.obj" \
	"$(INTDIR)\SDOXMLBufferWriter.obj" \
	"$(INTDIR)\SDOXMLFileWriter.obj" \
	"$(INTDIR)\SDOXMLStreamWriter.obj" \
	"$(INTDIR)\SDOXMLString.obj" \
	"$(INTDIR)\SDOXMLWriter.obj" \
	"$(INTDIR)\SDOXSDBufferWriter.obj" \
	"$(INTDIR)\SDOXSDFileWriter.obj" \
	"$(INTDIR)\SDOXSDStreamWriter.obj" \
	"$(INTDIR)\SDOXSDWriter.obj" \
	"$(INTDIR)\Sequence.obj" \
	"$(INTDIR)\SequenceImpl.obj" \
	"$(INTDIR)\Setting.obj" \
	"$(INTDIR)\SettingList.obj" \
	"$(INTDIR)\Type.obj" \
	"$(INTDIR)\TypeDefinition.obj" \
	"$(INTDIR)\TypeDefinitionImpl.obj" \
	"$(INTDIR)\TypeDefinitions.obj" \
	"$(INTDIR)\TypeDefinitionsImpl.obj" \
	"$(INTDIR)\TypeImpl.obj" \
	"$(INTDIR)\TypeList.obj" \
	"$(INTDIR)\XMLDocument.obj" \
	"$(INTDIR)\XMLDocumentImpl.obj" \
	"$(INTDIR)\XMLHelper.obj" \
	"$(INTDIR)\XMLHelperImpl.obj" \
	"$(INTDIR)\XMLQName.obj" \
	"$(INTDIR)\XpathHelper.obj" \
	"$(INTDIR)\XSDHelper.obj" \
	"$(INTDIR)\XSDHelperImpl.obj" \
	"$(INTDIR)\XSDPropertyInfo.obj" \
	"$(INTDIR)\XSDTypeInfo.obj"

"$(OUTDIR)\tuscany_sdo.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
PostBuild_Desc=copyout
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sdo.dll"
   copy Debug\tuscany_sdo.dll ..\..\..\deploy\bin
	copy Debug\tuscany_sdo.lib ..\..\..\deploy\lib
	copy ..\..\..\runtime\core\src\commonj\sdo\*.h ..\..\..\deploy\include\commonj\sdo
	copy Debug\tuscany_sdo.pdb ..\..\..\deploy\bin
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sdo_runtime.dep")
!INCLUDE "sdo_runtime.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_runtime.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_runtime - Win32 Release" || "$(CFG)" == "sdo_runtime - Win32 Debug"
SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangedDataObjectListImpl.cpp

"$(INTDIR)\ChangedDataObjectListImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummary.cpp

"$(INTDIR)\ChangeSummary.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryBuilder.cpp

"$(INTDIR)\ChangeSummaryBuilder.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\ChangeSummaryImpl.cpp

"$(INTDIR)\ChangeSummaryImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\CopyHelper.cpp

"$(INTDIR)\CopyHelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASProperty.cpp

"$(INTDIR)\DASProperty.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASType.cpp

"$(INTDIR)\DASType.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValue.cpp

"$(INTDIR)\DASValue.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DASValues.cpp

"$(INTDIR)\DASValues.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactory.cpp

"$(INTDIR)\DataFactory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataFactoryImpl.cpp

"$(INTDIR)\DataFactoryImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraph.cpp

"$(INTDIR)\DataGraph.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataGraphImpl.cpp

"$(INTDIR)\DataGraphImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObject.cpp

"$(INTDIR)\DataObject.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectImpl.cpp

"$(INTDIR)\DataObjectImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectInstance.cpp

"$(INTDIR)\DataObjectInstance.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectList.cpp

"$(INTDIR)\DataObjectList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DataObjectListImpl.cpp

"$(INTDIR)\DataObjectListImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\DefaultLogWriter.cpp

"$(INTDIR)\DefaultLogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\EqualityHelper.cpp

"$(INTDIR)\EqualityHelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupDefinition.cpp

"$(INTDIR)\GroupDefinition.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\GroupEvent.cpp

"$(INTDIR)\GroupEvent.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\HelperProvider.cpp

"$(INTDIR)\HelperProvider.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\Logger.cpp

"$(INTDIR)\Logger.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\LogWriter.cpp

"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\ParserErrorSetter.cpp

"$(INTDIR)\ParserErrorSetter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\Property.cpp

"$(INTDIR)\Property.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinition.cpp

"$(INTDIR)\PropertyDefinition.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyDefinitionImpl.cpp

"$(INTDIR)\PropertyDefinitionImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyImpl.cpp

"$(INTDIR)\PropertyImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertyList.cpp

"$(INTDIR)\PropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\PropertySetting.cpp

"$(INTDIR)\PropertySetting.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingObject.cpp

"$(INTDIR)\RefCountingObject.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\RefCountingPointer.cpp

"$(INTDIR)\RefCountingPointer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attribute.cpp

"$(INTDIR)\SAX2Attribute.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Attributes.cpp

"$(INTDIR)\SAX2Attributes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Namespaces.cpp

"$(INTDIR)\SAX2Namespaces.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SAX2Parser.cpp

"$(INTDIR)\SAX2Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SchemaInfo.cpp

"$(INTDIR)\SchemaInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoCheck.cpp

"$(INTDIR)\SdoCheck.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDODate.cpp

"$(INTDIR)\SDODate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SdoRuntime.cpp

"$(INTDIR)\SdoRuntime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDORuntimeException.cpp

"$(INTDIR)\SDORuntimeException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSAX2Parser.cpp

"$(INTDIR)\SDOSAX2Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOSchemaSAX2Parser.cpp

"$(INTDIR)\SDOSchemaSAX2Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOUtils.cpp

"$(INTDIR)\SDOUtils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLBufferWriter.cpp

"$(INTDIR)\SDOXMLBufferWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLFileWriter.cpp

"$(INTDIR)\SDOXMLFileWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLStreamWriter.cpp

"$(INTDIR)\SDOXMLStreamWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLString.cpp

"$(INTDIR)\SDOXMLString.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXMLWriter.cpp

"$(INTDIR)\SDOXMLWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDBufferWriter.cpp

"$(INTDIR)\SDOXSDBufferWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDFileWriter.cpp

"$(INTDIR)\SDOXSDFileWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDStreamWriter.cpp

"$(INTDIR)\SDOXSDStreamWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SDOXSDWriter.cpp

"$(INTDIR)\SDOXSDWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\Sequence.cpp

"$(INTDIR)\Sequence.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SequenceImpl.cpp

"$(INTDIR)\SequenceImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\Setting.cpp

"$(INTDIR)\Setting.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\SettingList.cpp

"$(INTDIR)\SettingList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\Type.cpp

"$(INTDIR)\Type.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinition.cpp

"$(INTDIR)\TypeDefinition.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionImpl.cpp

"$(INTDIR)\TypeDefinitionImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitions.cpp

"$(INTDIR)\TypeDefinitions.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeDefinitionsImpl.cpp

"$(INTDIR)\TypeDefinitionsImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeImpl.cpp

"$(INTDIR)\TypeImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\TypeList.cpp

"$(INTDIR)\TypeList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocument.cpp

"$(INTDIR)\XMLDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLDocumentImpl.cpp

"$(INTDIR)\XMLDocumentImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelper.cpp

"$(INTDIR)\XMLHelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLHelperImpl.cpp

"$(INTDIR)\XMLHelperImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XMLQName.cpp

"$(INTDIR)\XMLQName.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XpathHelper.cpp

"$(INTDIR)\XpathHelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelper.cpp

"$(INTDIR)\XSDHelper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDHelperImpl.cpp

"$(INTDIR)\XSDHelperImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDPropertyInfo.cpp

"$(INTDIR)\XSDPropertyInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\commonj\sdo\XSDTypeInfo.cpp

"$(INTDIR)\XSDTypeInfo.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

