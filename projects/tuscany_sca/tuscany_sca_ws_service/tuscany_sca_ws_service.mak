# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_ws_service.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_ws_service - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_ws_service - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_ws_service - Win32 Release" && "$(CFG)" != "tuscany_sca_ws_service - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_ws_service.mak" CFG="tuscany_sca_ws_service - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_ws_service - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_ws_service - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ws_service.dll"

!ELSE 

ALL : "tuscany_sca_ws_dispatcher - Win32 Release" "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ws_service.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" "tuscany_sca_ws_dispatcher - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axis2Service.obj"
	-@erase "$(INTDIR)\Axis2Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WSReferenceBinding.obj"
	-@erase "$(INTDIR)\WSReferenceBindingExtension.obj"
	-@erase "$(INTDIR)\WSServiceProxy.obj"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_SERVICE_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_service.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_ws_service.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_service.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_service.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\WSReferenceBinding.obj" \
	"$(INTDIR)\Axis2Service.obj" \
	"$(INTDIR)\Axis2Utils.obj" \
	"$(INTDIR)\WSReferenceBindingExtension.obj" \
	"$(INTDIR)\WSServiceProxy.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib" \
	"..\tuscany_sca_ws_dispatcher\Release\tuscany_sca_ws_dispatcher.lib"

"$(OUTDIR)\tuscany_sca_ws_service.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca_ws_dispatcher - Win32 Release" "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ws_service.dll"
   ..\..\..\runtime\extensions\ws\service\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ws_service.dll"

!ELSE 

ALL : "tuscany_sca_ws_dispatcher - Win32 Debug" "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ws_service.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" "tuscany_sca_ws_dispatcher - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axis2Service.obj"
	-@erase "$(INTDIR)\Axis2Utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WSReferenceBinding.obj"
	-@erase "$(INTDIR)\WSReferenceBindingExtension.obj"
	-@erase "$(INTDIR)\WSServiceProxy.obj"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.lib"
	-@erase "$(OUTDIR)\tuscany_sca_ws_service.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_SERVICE_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_service.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_ws_service.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_service.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_service.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\WSReferenceBinding.obj" \
	"$(INTDIR)\Axis2Service.obj" \
	"$(INTDIR)\Axis2Utils.obj" \
	"$(INTDIR)\WSReferenceBindingExtension.obj" \
	"$(INTDIR)\WSServiceProxy.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib" \
	"..\tuscany_sca_ws_dispatcher\Debug\tuscany_sca_ws_dispatcher.lib"

"$(OUTDIR)\tuscany_sca_ws_service.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca_ws_dispatcher - Win32 Debug" "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ws_service.dll"
   ..\..\..\runtime\extensions\ws\service\deploy.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_ws_service.dep")
!INCLUDE "tuscany_sca_ws_service.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_ws_service.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_ws_service - Win32 Release" || "$(CFG)" == "tuscany_sca_ws_service - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\model\WSReferenceBinding.cpp

"$(INTDIR)\WSReferenceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2Service.cpp

"$(INTDIR)\Axis2Service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2Utils.cpp

"$(INTDIR)\Axis2Utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\WSReferenceBindingExtension.cpp

"$(INTDIR)\WSReferenceBindingExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\WSServiceProxy.cpp

"$(INTDIR)\WSServiceProxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_ws_service"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_service"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_ws_service"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_service"

!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Release"

"tuscany_sca_ws_dispatcher - Win32 Release" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Release" 
   cd "..\tuscany_sca_ws_service"

"tuscany_sca_ws_dispatcher - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_service"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_service - Win32 Debug"

"tuscany_sca_ws_dispatcher - Win32 Debug" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Debug" 
   cd "..\tuscany_sca_ws_service"

"tuscany_sca_ws_dispatcher - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca_ws_dispatcher"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca_ws_dispatcher.mak CFG="tuscany_sca_ws_dispatcher - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_service"

!ENDIF 


!ENDIF 

