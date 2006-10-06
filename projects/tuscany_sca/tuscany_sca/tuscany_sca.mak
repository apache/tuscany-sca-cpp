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
	-@erase "$(INTDIR)\Binding.obj"
	-@erase "$(INTDIR)\Component.obj"
	-@erase "$(INTDIR)\ComponentType.obj"
	-@erase "$(INTDIR)\Composite.obj"
	-@erase "$(INTDIR)\CompositeReference.obj"
	-@erase "$(INTDIR)\CompositeService.obj"
	-@erase "$(INTDIR)\Contract.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\Exceptions.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\FileLogWriter.obj"
	-@erase "$(INTDIR)\ImplementationExtension.obj"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\InterfaceExtension.obj"
	-@erase "$(INTDIR)\Library.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ModelLoader.obj"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\Reference.obj"
	-@erase "$(INTDIR)\ReferenceBinding.obj"
	-@erase "$(INTDIR)\ReferenceBindingExtension.obj"
	-@erase "$(INTDIR)\ReferenceType.obj"
	-@erase "$(INTDIR)\SCARuntime.obj"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\ServiceBinding.obj"
	-@erase "$(INTDIR)\ServiceBindingExtension.obj"
	-@erase "$(INTDIR)\ServiceProxy.obj"
	-@erase "$(INTDIR)\ServiceType.obj"
	-@erase "$(INTDIR)\ServiceWrapper.obj"
	-@erase "$(INTDIR)\TuscanyRuntime.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Wire.obj"
	-@erase "$(INTDIR)\WSDLDefinition.obj"
	-@erase "$(INTDIR)\WSDLInterface.obj"
	-@erase "$(INTDIR)\WSDLOperation.obj"
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
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\SCARuntime.obj" \
	"$(INTDIR)\ServiceProxy.obj" \
	"$(INTDIR)\ServiceWrapper.obj" \
	"$(INTDIR)\TuscanyRuntime.obj" \
	"$(INTDIR)\Binding.obj" \
	"$(INTDIR)\Component.obj" \
	"$(INTDIR)\ComponentType.obj" \
	"$(INTDIR)\Composite.obj" \
	"$(INTDIR)\CompositeReference.obj" \
	"$(INTDIR)\CompositeService.obj" \
	"$(INTDIR)\Contract.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\ModelLoader.obj" \
	"$(INTDIR)\Reference.obj" \
	"$(INTDIR)\ReferenceBinding.obj" \
	"$(INTDIR)\ReferenceType.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\ServiceBinding.obj" \
	"$(INTDIR)\ServiceType.obj" \
	"$(INTDIR)\Wire.obj" \
	"$(INTDIR)\WSDLDefinition.obj" \
	"$(INTDIR)\WSDLInterface.obj" \
	"$(INTDIR)\WSDLOperation.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\Exceptions.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileLogWriter.obj" \
	"$(INTDIR)\Library.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\ImplementationExtension.obj" \
	"$(INTDIR)\InterfaceExtension.obj" \
	"$(INTDIR)\ReferenceBindingExtension.obj" \
	"$(INTDIR)\ServiceBindingExtension.obj"

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
   ..\..\..\runtime\core\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca.dll"


CLEAN :
	-@erase "$(INTDIR)\Binding.obj"
	-@erase "$(INTDIR)\Component.obj"
	-@erase "$(INTDIR)\ComponentType.obj"
	-@erase "$(INTDIR)\Composite.obj"
	-@erase "$(INTDIR)\CompositeReference.obj"
	-@erase "$(INTDIR)\CompositeService.obj"
	-@erase "$(INTDIR)\Contract.obj"
	-@erase "$(INTDIR)\DefaultLogWriter.obj"
	-@erase "$(INTDIR)\Exceptions.obj"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\FileLogWriter.obj"
	-@erase "$(INTDIR)\ImplementationExtension.obj"
	-@erase "$(INTDIR)\Interface.obj"
	-@erase "$(INTDIR)\InterfaceExtension.obj"
	-@erase "$(INTDIR)\Library.obj"
	-@erase "$(INTDIR)\Logger.obj"
	-@erase "$(INTDIR)\LogWriter.obj"
	-@erase "$(INTDIR)\ModelLoader.obj"
	-@erase "$(INTDIR)\Operation.obj"
	-@erase "$(INTDIR)\Reference.obj"
	-@erase "$(INTDIR)\ReferenceBinding.obj"
	-@erase "$(INTDIR)\ReferenceBindingExtension.obj"
	-@erase "$(INTDIR)\ReferenceType.obj"
	-@erase "$(INTDIR)\SCARuntime.obj"
	-@erase "$(INTDIR)\Service.obj"
	-@erase "$(INTDIR)\ServiceBinding.obj"
	-@erase "$(INTDIR)\ServiceBindingExtension.obj"
	-@erase "$(INTDIR)\ServiceProxy.obj"
	-@erase "$(INTDIR)\ServiceType.obj"
	-@erase "$(INTDIR)\ServiceWrapper.obj"
	-@erase "$(INTDIR)\TuscanyRuntime.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wire.obj"
	-@erase "$(INTDIR)\WSDLDefinition.obj"
	-@erase "$(INTDIR)\WSDLInterface.obj"
	-@erase "$(INTDIR)\WSDLOperation.obj"
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
	"$(INTDIR)\Operation.obj" \
	"$(INTDIR)\SCARuntime.obj" \
	"$(INTDIR)\ServiceProxy.obj" \
	"$(INTDIR)\ServiceWrapper.obj" \
	"$(INTDIR)\TuscanyRuntime.obj" \
	"$(INTDIR)\Binding.obj" \
	"$(INTDIR)\Component.obj" \
	"$(INTDIR)\ComponentType.obj" \
	"$(INTDIR)\Composite.obj" \
	"$(INTDIR)\CompositeReference.obj" \
	"$(INTDIR)\CompositeService.obj" \
	"$(INTDIR)\Contract.obj" \
	"$(INTDIR)\Interface.obj" \
	"$(INTDIR)\ModelLoader.obj" \
	"$(INTDIR)\Reference.obj" \
	"$(INTDIR)\ReferenceBinding.obj" \
	"$(INTDIR)\ReferenceType.obj" \
	"$(INTDIR)\Service.obj" \
	"$(INTDIR)\ServiceBinding.obj" \
	"$(INTDIR)\ServiceType.obj" \
	"$(INTDIR)\Wire.obj" \
	"$(INTDIR)\WSDLDefinition.obj" \
	"$(INTDIR)\WSDLInterface.obj" \
	"$(INTDIR)\WSDLOperation.obj" \
	"$(INTDIR)\DefaultLogWriter.obj" \
	"$(INTDIR)\Exceptions.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FileLogWriter.obj" \
	"$(INTDIR)\Library.obj" \
	"$(INTDIR)\Logger.obj" \
	"$(INTDIR)\LogWriter.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\ImplementationExtension.obj" \
	"$(INTDIR)\InterfaceExtension.obj" \
	"$(INTDIR)\ReferenceBindingExtension.obj" \
	"$(INTDIR)\ServiceBindingExtension.obj"

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
   ..\..\..\runtime\core\deploy.bat ..\..\..\ Debug
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
SOURCE=..\..\..\runtime\core\src\tuscany\sca\core\Operation.cpp

"$(INTDIR)\Operation.obj" : $(SOURCE) "$(INTDIR)"
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


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ComponentType.cpp

"$(INTDIR)\ComponentType.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Composite.cpp

"$(INTDIR)\Composite.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeReference.cpp

"$(INTDIR)\CompositeReference.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\CompositeService.cpp

"$(INTDIR)\CompositeService.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Contract.cpp

"$(INTDIR)\Contract.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Interface.cpp

"$(INTDIR)\Interface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ModelLoader.cpp

"$(INTDIR)\ModelLoader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Reference.cpp

"$(INTDIR)\Reference.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceBinding.cpp

"$(INTDIR)\ReferenceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ReferenceType.cpp

"$(INTDIR)\ReferenceType.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Service.cpp

"$(INTDIR)\Service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceBinding.cpp

"$(INTDIR)\ServiceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\ServiceType.cpp

"$(INTDIR)\ServiceType.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\Wire.cpp

"$(INTDIR)\Wire.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLDefinition.cpp

"$(INTDIR)\WSDLDefinition.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLInterface.cpp

"$(INTDIR)\WSDLInterface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\model\WSDLOperation.cpp

"$(INTDIR)\WSDLOperation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\DefaultLogWriter.cpp

"$(INTDIR)\DefaultLogWriter.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\util\Exceptions.cpp

"$(INTDIR)\Exceptions.obj" : $(SOURCE) "$(INTDIR)"
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


SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ImplementationExtension.cpp

"$(INTDIR)\ImplementationExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\InterfaceExtension.cpp

"$(INTDIR)\InterfaceExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ReferenceBindingExtension.cpp

"$(INTDIR)\ReferenceBindingExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\src\tuscany\sca\extension\ServiceBindingExtension.cpp

"$(INTDIR)\ServiceBindingExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

