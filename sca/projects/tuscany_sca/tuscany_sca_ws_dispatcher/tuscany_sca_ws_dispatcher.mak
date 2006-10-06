# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_ws_dispatcher.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_ws_dispatcher - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_ws_dispatcher - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_ws_dispatcher - Win32 Release" && "$(CFG)" != "tuscany_sca_ws_dispatcher - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"


CLEAN :
	-@erase "$(INTDIR)\Axis2Dispatcher.obj"
	-@erase "$(INTDIR)\Axis2DispatcherModule.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_ws_dispatcher_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_dispatcher.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_ws_dispatcher.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_dispatcher.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_dispatcher.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\Axis2Dispatcher.obj" \
	"$(INTDIR)\Axis2DispatcherModule.obj"

"$(OUTDIR)\tuscany_sca_ws_dispatcher.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"
   ..\..\..\runtime\extensions\ws\service\deploymodule.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"


CLEAN :
	-@erase "$(INTDIR)\Axis2Dispatcher.obj"
	-@erase "$(INTDIR)\Axis2DispatcherModule.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.lib"
	-@erase "$(OUTDIR)\tuscany_sca_ws_dispatcher.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/ws/service/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_DISPATCHER_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_dispatcher.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_ws_dispatcher.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_dispatcher.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_dispatcher.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\Axis2Dispatcher.obj" \
	"$(INTDIR)\Axis2DispatcherModule.obj"

"$(OUTDIR)\tuscany_sca_ws_dispatcher.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\tuscany_sca_ws_dispatcher.dll"
   ..\..\..\runtime\extensions\ws\service\deploymodule.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_ws_dispatcher.dep")
!INCLUDE "tuscany_sca_ws_dispatcher.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_ws_dispatcher.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Release" || "$(CFG)" == "tuscany_sca_ws_dispatcher - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2Dispatcher.cpp

"$(INTDIR)\Axis2Dispatcher.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\service\axis2c\src\tuscany\sca\ws\Axis2DispatcherModule.cpp

"$(INTDIR)\Axis2DispatcherModule.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

