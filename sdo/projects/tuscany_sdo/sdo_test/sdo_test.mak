# Microsoft Developer Studio Generated NMAKE File, Based on sdo_test.dsp
!IF "$(CFG)" == ""
CFG=sdo_test - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sdo_test - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sdo_test - Win32 Release" && "$(CFG)" != "sdo_test - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdo_test.mak" CFG="sdo_test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdo_test - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "sdo_test - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sdo_test - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sdo_test.exe"

!ELSE 

ALL : "sdo_runtime - Win32 Release" "$(OUTDIR)\sdo_test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\SdoGenerate.obj"
	-@erase "$(INTDIR)\sdotest.obj"
	-@erase "$(INTDIR)\sdotest2.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\sdo_test.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /I "..\..\..\deploy\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\sdo_test.pdb" /machine:I386 /out:"$(OUTDIR)\sdo_test.exe" /libpath:"..\..\..\deploy\lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\SdoGenerate.obj" \
	"$(INTDIR)\sdotest.obj" \
	"$(INTDIR)\sdotest2.obj" \
	"$(INTDIR)\utils.obj" \
	"..\sdo_runtime\Release\tuscany_sdo.lib"

"$(OUTDIR)\sdo_test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Release" "$(OUTDIR)\sdo_test.exe"
   copy ..\..\..\deploy\bin\*.dll Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sdo_test.exe"

!ELSE 

ALL : "sdo_runtime - Win32 Debug" "$(OUTDIR)\sdo_test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\SdoGenerate.obj"
	-@erase "$(INTDIR)\sdotest.obj"
	-@erase "$(INTDIR)\sdotest2.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sdo_test.exe"
	-@erase "$(OUTDIR)\sdo_test.ilk"
	-@erase "$(OUTDIR)\sdo_test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\deploy\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Zm200 /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\sdo_test.pdb" /debug /machine:I386 /out:"$(OUTDIR)\sdo_test.exe" /pdbtype:sept /libpath:"..\..\..\deploy\lib" 
LINK32_OBJS= \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\SdoGenerate.obj" \
	"$(INTDIR)\sdotest.obj" \
	"$(INTDIR)\sdotest2.obj" \
	"$(INTDIR)\utils.obj" \
	"..\sdo_runtime\Debug\tuscany_sdo.lib"

"$(OUTDIR)\sdo_test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Debug" "$(OUTDIR)\sdo_test.exe"
   copy ..\..\..\deploy\bin\*.dll Debug
	copy ..\..\..\deploy\bin\*.pdb Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sdo_test.dep")
!INCLUDE "sdo_test.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_test.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_test - Win32 Release" || "$(CFG)" == "sdo_test - Win32 Debug"
SOURCE=..\..\..\runtime\core\test\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\SdoGenerate.cpp

"$(INTDIR)\SdoGenerate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\sdotest.cpp

"$(INTDIR)\sdotest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\sdotest2.cpp

"$(INTDIR)\sdotest2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\test\utils.cpp

"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "sdo_test - Win32 Release"

"sdo_runtime - Win32 Release" : 
   cd "\distro\sdo\projects\tuscany_sdo\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" 
   cd "..\sdo_test"

"sdo_runtime - Win32 ReleaseCLEAN" : 
   cd "\distro\sdo\projects\tuscany_sdo\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" RECURSE=1 CLEAN 
   cd "..\sdo_test"

!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"

"sdo_runtime - Win32 Debug" : 
   cd "\distro\sdo\projects\tuscany_sdo\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" 
   cd "..\sdo_test"

"sdo_runtime - Win32 DebugCLEAN" : 
   cd "\distro\sdo\projects\tuscany_sdo\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\sdo_test"

!ENDIF 


!ENDIF 

