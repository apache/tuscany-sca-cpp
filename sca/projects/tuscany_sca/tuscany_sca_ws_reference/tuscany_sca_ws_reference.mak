# Microsoft Developer Studio Generated NMAKE File, Based on tuscany_sca_ws_reference.dsp
!IF "$(CFG)" == ""
CFG=tuscany_sca_ws_reference - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tuscany_sca_ws_reference - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tuscany_sca_ws_reference - Win32 Release" && "$(CFG)" != "tuscany_sca_ws_reference - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tuscany_sca_ws_reference.mak" CFG="tuscany_sca_ws_reference - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tuscany_sca_ws_reference - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tuscany_sca_ws_reference - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tuscany_sca_ws_reference - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ws_reference.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ws_reference.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axis2Client.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WSServiceBinding.obj"
	-@erase "$(INTDIR)\WSServiceBindingExtension.obj"
	-@erase "$(INTDIR)\WSServiceWrapper.obj"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../../../runtime/extensions/ws/reference/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_REFERENCE_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_reference.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib axis2_parser.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tuscany_sca_ws_reference.pdb" /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_reference.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_reference.lib" /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\WSServiceBinding.obj" \
	"$(INTDIR)\Axis2Client.obj" \
	"$(INTDIR)\WSServiceBindingExtension.obj" \
	"$(INTDIR)\WSServiceWrapper.obj" \
	"..\tuscany_sca\Release\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_ws_reference.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Release" "$(OUTDIR)\tuscany_sca_ws_reference.dll"
   ..\..\..\runtime\extensions\ws\reference\deploy.bat ..\..\..\ Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_reference - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sca_ws_reference.dll"

!ELSE 

ALL : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ws_reference.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"tuscany_sca - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Axis2Client.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WSServiceBinding.obj"
	-@erase "$(INTDIR)\WSServiceBindingExtension.obj"
	-@erase "$(INTDIR)\WSServiceWrapper.obj"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.dll"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.exp"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.ilk"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.lib"
	-@erase "$(OUTDIR)\tuscany_sca_ws_reference.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../runtime/extensions/ws/reference/axis2c/src" /I "../../../runtime/core/src" /I "$(TUSCANY_SDOCPP)/include" /I "$(AXIS2C_HOME)/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TUSCANY_SCA_WS_REFERENCE_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tuscany_sca_ws_reference.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_util.lib axiom.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib axis2_engine.lib tuscany_sdo_axiom.lib tuscany_sca.lib tuscany_sdo.lib axis2_parser.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sca_ws_reference.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sca_ws_reference.dll" /implib:"$(OUTDIR)\tuscany_sca_ws_reference.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(TUSCANY_SDOCPP)\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\WSServiceBinding.obj" \
	"$(INTDIR)\Axis2Client.obj" \
	"$(INTDIR)\WSServiceBindingExtension.obj" \
	"$(INTDIR)\WSServiceWrapper.obj" \
	"..\tuscany_sca\Debug\tuscany_sca.lib"

"$(OUTDIR)\tuscany_sca_ws_reference.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "tuscany_sca - Win32 Debug" "$(OUTDIR)\tuscany_sca_ws_reference.dll"
   ..\..\..\runtime\extensions\ws\reference\deploy.bat ..\..\..\ Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tuscany_sca_ws_reference.dep")
!INCLUDE "tuscany_sca_ws_reference.dep"
!ELSE 
!MESSAGE Warning: cannot find "tuscany_sca_ws_reference.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tuscany_sca_ws_reference - Win32 Release" || "$(CFG)" == "tuscany_sca_ws_reference - Win32 Debug"
SOURCE=..\..\..\runtime\extensions\ws\reference\axis2c\src\tuscany\sca\ws\model\WSServiceBinding.cpp

"$(INTDIR)\WSServiceBinding.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\reference\axis2c\src\tuscany\sca\ws\Axis2Client.cpp

"$(INTDIR)\Axis2Client.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\reference\axis2c\src\tuscany\sca\ws\WSServiceBindingExtension.cpp

"$(INTDIR)\WSServiceBindingExtension.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\extensions\ws\reference\axis2c\src\tuscany\sca\ws\WSServiceWrapper.cpp

"$(INTDIR)\WSServiceWrapper.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "tuscany_sca_ws_reference - Win32 Release"

"tuscany_sca - Win32 Release" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" 
   cd "..\tuscany_sca_ws_reference"

"tuscany_sca - Win32 ReleaseCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Release" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_reference"

!ELSEIF  "$(CFG)" == "tuscany_sca_ws_reference - Win32 Debug"

"tuscany_sca - Win32 Debug" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" 
   cd "..\tuscany_sca_ws_reference"

"tuscany_sca - Win32 DebugCLEAN" : 
   cd "..\tuscany_sca"
   $(MAKE) /$(MAKEFLAGS) /F .\tuscany_sca.mak CFG="tuscany_sca - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\tuscany_sca_ws_reference"

!ENDIF 


!ENDIF 

