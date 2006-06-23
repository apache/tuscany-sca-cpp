# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca - Win32 Release" && "$(CFG)" != "tuscany_sca - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca.dll"


CLEAN :
	-@erase "$(INTDIR)\Axis2Client.obj"
	-@erase "$(INTDIR)\Binding.obj"
	-@erase "$(INTDIR)\Component.obj"
	-@erase "$(INTDIR)\ComponentContext.obj"
	-@erase "$(INTDIR)\ComponentContextImpl.obj"
	-@erase "$(INTDIR)\ComponentServiceWrapper.obj"
	-@erase "$(INTDIR)\CPPImplementation.obj"
	-@erase "$(INTDIR)\CPPInterface.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\EntryPoint.obj"
	-@erase "$(INTDIR)\EntryPointProxy.obj"
	-@erase "$(INTDIR)\ExternalService.obj"
	-@erase "$(INTDIR)\ExternalServiceWrapper.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\FileLogWriter.obj"
	-@erase "$(INTDIR)\Implementation.obj"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Library.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ModelLoader.obj"
	-@erase "$(INTDIR)\Module.obj"
	-@erase "$(INTDIR)\ModuleContext.obj"
	-@erase "$(INTDIR)\ModuleContextImpl.obj"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\SCA.obj"
	-@erase "$(INTDIR)\SCAEntryPoint.obj"
	-@erase "$(INTDIR)\SCARuntime.obj"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\ServiceList.obj"
	-@erase "$(INTDIR)\ServiceProxy.obj"
	-@erase "$(INTDIR)\ServiceReference.obj"
	-@erase "$(INTDIR)\ServiceRuntimeException.obj"
	-@erase "$(INTDIR)\ServiceWrapper.obj"
	-@erase "$(INTDIR)\Subsystem.obj"
	-@erase "$(INTDIR)\System.obj"
	-@erase "$(INTDIR)\TuscanyRuntime.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Wire.obj"
	-@erase "$(INTDIR)\WireTarget.obj"
	-@erase "$(INTDIR)\WSBinding.obj"
	-@erase "$(INTDIR)\Wsdl.obj"
	-@erase "$(INTDIR)\WsdlOperation.obj"
	-@erase "$(INTDIR)\WSServiceWrapper.obj"
	-@erase "$(OUTDIR)\tuscany_sca.dll"
	-@erase "$(OUTDIR)\tuscany_sca.exp"
	-@erase "$(OUTDIR)\tuscany_sca.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCA_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_engine.lib tuscany_sdo_axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca.dll" /implib:"$(OUTDIR)\tuscany_sca.lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ComponentContext.obj" \
	"$(INTDIR)\ComponentContextImpl.obj" \
	"$(INTDIR)\ModuleContext.obj" \
	"$(INTDIR)\ModuleContextImpl.obj" \
	"$(INTDIR)\SCA.obj" \
	"$(INTDIR)\ServiceList.obj" \
	"$(INTDIR)\ServiceRuntimeException.obj" \
	"$(INTDIR)\ComponentServiceWrapper.obj" \
	"$(INTDIR)\ExternalServiceWrapper.obj" \
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\SCAEntryPoint.obj" \
	"$(INTDIR)\SCARuntime.obj" \
	"$(INTDIR)\ServiceProxy.obj" \
	"$(INTDIR)\ServiceWrapper.obj" \
	"$(INTDIR)\TuscanyRuntime.obj" \
	"$(INTDIR)\Binding.obj" \
	"$(INTDIR)\Component.obj" \
	"$(INTDIR)\CPPImplementation.obj" \
	"$(INTDIR)\CPPInterface.obj" \
	"$(INTDIR)\EntryPoint.obj" \
	"$(INTDIR)\ExternalService.obj" \
	"$(INTDIR)\Implementation.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\ModelLoader.obj" \
	"$(INTDIR)\Module.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\ServiceReference.obj" \
	"$(INTDIR)\Subsystem.obj" \
	"$(INTDIR)\System.obj" \
	"$(INTDIR)\Wire.obj" \
	"$(INTDIR)\WireTarget.obj" \
	"$(INTDIR)\WSBinding.obj" \
	"$(INTDIR)\Wsdl.obj" \
	"$(INTDIR)\WsdlOperation.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileLogWriter.obj" \
	"$(INTDIR)\Library.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\Axis2Client.obj" \
	"$(INTDIR)\EntryPointProxy.obj" \
	"$(INTDIR)\WSServiceWrapper.obj"

"$(OUTDIR)\tuscany_sca.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca.dll"
   del ..\..\..\deploy\bin\tuscany_sca.*
	del ..\..\..\deploy\lib\*.lib
	copy ..\..\..\runtime\core\src\osoa\sca\*.h ..\..\..\deploy\include\osoa\sca
	copy ..\..\..\runtime\core\src\tuscany\sca\core\*.h ..\..\..\deploy\include\tuscany\sca\core
	copy ..\..\..\runtime\core\src\tuscany\sca\model\*.h ..\..\..\deploy\include\tuscany\sca\model
	copy ..\..\..\runtime\core\src\tuscany\sca\util\*.h ..\..\..\deploy\include\tuscany\sca\util
	copy Release\tuscany_sca.dll ..\..\..\deploy\bin
	copy Release\tuscany_sca.lib ..\..\..\deploy\lib
	copy ..\..\..\runtime\core\src\tuscany\sca\ws\*.h ..\..\..\deploy\include\tuscany\sca\ws
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca.dll"


CLEAN :
	-@erase "$(INTDIR)\Axis2Client.obj"
	-@erase "$(INTDIR)\Binding.obj"
	-@erase "$(INTDIR)\Component.obj"
	-@erase "$(INTDIR)\ComponentContext.obj"
	-@erase "$(INTDIR)\ComponentContextImpl.obj"
	-@erase "$(INTDIR)\ComponentServiceWrapper.obj"
	-@erase "$(INTDIR)\CPPImplementation.obj"
	-@erase "$(INTDIR)\CPPInterface.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\EntryPoint.obj"
	-@erase "$(INTDIR)\EntryPointProxy.obj"
	-@erase "$(INTDIR)\ExternalService.obj"
	-@erase "$(INTDIR)\ExternalServiceWrapper.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\FileLogWriter.obj"
	-@erase "$(INTDIR)\Implementation.obj"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\Library.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ModelLoader.obj"
	-@erase "$(INTDIR)\Module.obj"
	-@erase "$(INTDIR)\ModuleContext.obj"
	-@erase "$(INTDIR)\ModuleContextImpl.obj"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\SCA.obj"
	-@erase "$(INTDIR)\SCAEntryPoint.obj"
	-@erase "$(INTDIR)\SCARuntime.obj"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\ServiceList.obj"
	-@erase "$(INTDIR)\ServiceProxy.obj"
	-@erase "$(INTDIR)\ServiceReference.obj"
	-@erase "$(INTDIR)\ServiceRuntimeException.obj"
	-@erase "$(INTDIR)\ServiceWrapper.obj"
	-@erase "$(INTDIR)\Subsystem.obj"
	-@erase "$(INTDIR)\System.obj"
	-@erase "$(INTDIR)\TuscanyRuntime.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wire.obj"
	-@erase "$(INTDIR)\WireTarget.obj"
	-@erase "$(INTDIR)\WSBinding.obj"
	-@erase "$(INTDIR)\Wsdl.obj"
	-@erase "$(INTDIR)\WsdlOperation.obj"
	-@erase "$(INTDIR)\WSServiceWrapper.obj"
	-@erase "$(OUTDIR)\tuscany_sca.dll"
	-@erase "$(OUTDIR)\tuscany_sca.exp"
	-@erase "$(OUTDIR)\tuscany_sca.ilk"
	-@erase "$(OUTDIR)\tuscany_sca.lib"
	-@erase "$(OUTDIR)\tuscany_sca.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCA_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_engine.lib tuscany_sdo_axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib axis2_parser.lib axis2_util.lib axiom.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca.dll" /implib:"$(OUTDIR)\tuscany_sca.lib" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ComponentContext.obj" \
	"$(INTDIR)\ComponentContextImpl.obj" \
	"$(INTDIR)\ModuleContext.obj" \
	"$(INTDIR)\ModuleContextImpl.obj" \
	"$(INTDIR)\SCA.obj" \
	"$(INTDIR)\ServiceList.obj" \
	"$(INTDIR)\ServiceRuntimeException.obj" \
	"$(INTDIR)\ComponentServiceWrapper.obj" \
	"$(INTDIR)\ExternalServiceWrapper.obj" \
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\SCAEntryPoint.obj" \
	"$(INTDIR)\SCARuntime.obj" \
	"$(INTDIR)\ServiceProxy.obj" \
	"$(INTDIR)\ServiceWrapper.obj" \
	"$(INTDIR)\TuscanyRuntime.obj" \
	"$(INTDIR)\Binding.obj" \
	"$(INTDIR)\Component.obj" \
	"$(INTDIR)\CPPImplementation.obj" \
	"$(INTDIR)\CPPInterface.obj" \
	"$(INTDIR)\EntryPoint.obj" \
	"$(INTDIR)\ExternalService.obj" \
	"$(INTDIR)\Implementation.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\ModelLoader.obj" \
	"$(INTDIR)\Module.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\ServiceReference.obj" \
	"$(INTDIR)\Subsystem.obj" \
	"$(INTDIR)\System.obj" \
	"$(INTDIR)\Wire.obj" \
	"$(INTDIR)\WireTarget.obj" \
	"$(INTDIR)\WSBinding.obj" \
	"$(INTDIR)\Wsdl.obj" \
	"$(INTDIR)\WsdlOperation.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileLogWriter.obj" \
	"$(INTDIR)\Library.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\Axis2Client.obj" \
	"$(INTDIR)\EntryPointProxy.obj" \
	"$(INTDIR)\WSServiceWrapper.obj"

"$(OUTDIR)\tuscany_sca.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca.dll"
   del ..\..\..\deploy\bin\tuscany_sca.*
	del ..\..\..\deploy\lib\*.lib
	copy ..\..\..\runtime\core\src\osoa\sca\*.h ..\..\..\deploy\include\osoa\sca
	copy ..\..\..\runtime\core\src\tuscany\sca\core\*.h ..\..\..\deploy\include\tuscany\sca\core
	copy ..\..\..\runtime\core\src\tuscany\sca\model\*.h ..\..\..\deploy\include\tuscany\sca\model
	copy ..\..\..\runtime\core\src\tuscany\sca\util\*.h ..\..\..\deploy\include\tuscany\sca\util
	copy ..\..\..\runtime\core\src\tuscany\sca\ws\*.h ..\..\..\deploy\include\tuscany\sca\ws
	copy Debug\tuscany_sca.dll ..\..\..\deploy\bin
	copy Debug\tuscany_sca.pdb ..\..\..\deploy\bin
	copy Debug\tuscany_sca.lib ..\..\..\deploy\lib
	copy ..\..\..\xsd\*.* ..\..\..\deploy\xsd
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca.dep")
!INCLUDE "tuscany_sca.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca - Win32 Release" || "$(CFG)" == "tuscany_sca - Win32 Debug"
SOURCE=..\..\..\runtime\core\src\osoa\sca\ComponentContext.cpp

"$(INTDIR)\ComponentContext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\ComponentContextImpl.cpp

"$(INTDIR)\ComponentContextImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\ModuleContext.cpp

"$(INTDIR)\ModuleContext.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\ModuleContextImpl.cpp

"$(INTDIR)\ModuleContextImpl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\SCA.cpp

"$(INTDIR)\SCA.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\ServiceList.cpp

"$(INTDIR)\ServiceList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\osoa\sca\ServiceRuntimeException.cpp

"$(INTDIR)\ServiceRuntimeException.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ComponentServiceWrapper.cpp

"$(INTDIR)\ComponentServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ExternalServiceWrapper.cpp

"$(INTDIR)\ExternalServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\Operation.cpp

"$(INTDIR)\Operation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\SCAEntryPoint.cpp

"$(INTDIR)\SCAEntryPoint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\SCARuntime.cpp

"$(INTDIR)\SCARuntime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceProxy.cpp

"$(INTDIR)\ServiceProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\ServiceWrapper.cpp

"$(INTDIR)\ServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\TuscanyRuntime.cpp

"$(INTDIR)\TuscanyRuntime.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Binding.cpp

"$(INTDIR)\Binding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Component.cpp

"$(INTDIR)\Component.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CPPImplementation.cpp

"$(INTDIR)\CPPImplementation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CPPInterface.cpp

"$(INTDIR)\CPPInterface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\EntryPoint.cpp

"$(INTDIR)\EntryPoint.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ExternalService.cpp

"$(INTDIR)\ExternalService.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Implementation.cpp

"$(INTDIR)\Implementation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Interface.cpp

"$(INTDIR)\Interface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ModelLoader.cpp

"$(INTDIR)\ModelLoader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Module.cpp

"$(INTDIR)\Module.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Service.cpp

"$(INTDIR)\Service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceReference.cpp

"$(INTDIR)\ServiceReference.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Subsystem.cpp

"$(INTDIR)\Subsystem.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\System.cpp

"$(INTDIR)\System.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Wire.cpp

"$(INTDIR)\Wire.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WireTarget.cpp

"$(INTDIR)\WireTarget.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSBinding.cpp

"$(INTDIR)\WSBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Wsdl.cpp

"$(INTDIR)\Wsdl.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WsdlOperation.cpp

"$(INTDIR)\WsdlOperation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\DefaultLogWriter.cpp

"$(INTDIR)\DefaultLogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\File.cpp

"$(INTDIR)\File.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\FileLogWriter.cpp

"$(INTDIR)\FileLogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Library.cpp

"$(INTDIR)\Library.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Logger.cpp

"$(INTDIR)\Logger.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\LogWriter.cpp

"$(INTDIR)\LogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Utils.cpp

"$(INTDIR)\Utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\ws_reference\axis2c\src\Axis2Client.cpp

"$(INTDIR)\Axis2Client.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\ws\EntryPointProxy.cpp

"$(INTDIR)\EntryPointProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\ws_reference\axis2c\src\WSServiceWrapper.cpp

"$(INTDIR)\WSServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

