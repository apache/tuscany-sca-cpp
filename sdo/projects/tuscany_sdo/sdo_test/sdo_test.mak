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

CPP=cl.exe
RSC=rc.exe

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

CPP_PROJ=/nologo /MD /W3 /GX /I "..\..\..\deploy\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
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

ALL : "$(OUTDIR)\sdo_test.exe" "$(OUTDIR)\sdo_test.bsc"

!ELSE 

ALL : "sdo_runtime - Win32 Debug" "$(OUTDIR)\sdo_test.exe" "$(OUTDIR)\sdo_test.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\SdoGenerate.obj"
	-@erase "$(INTDIR)\SdoGenerate.sbr"
	-@erase "$(INTDIR)\sdotest.obj"
	-@erase "$(INTDIR)\sdotest.sbr"
	-@erase "$(INTDIR)\sdotest2.obj"
	-@erase "$(INTDIR)\sdotest2.sbr"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sdo_test.bsc"
	-@erase "$(OUTDIR)\sdo_test.exe"
	-@erase "$(OUTDIR)\sdo_test.ilk"
	-@erase "$(OUTDIR)\sdo_test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\deploy\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /Zm200 /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_test.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\SdoGenerate.sbr" \
	"$(INTDIR)\sdotest.sbr" \
	"$(INTDIR)\sdotest2.sbr" \
	"$(INTDIR)\utils.sbr"

"$(OUTDIR)\sdo_test.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

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

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Debug" "$(OUTDIR)\sdo_test.exe" "$(OUTDIR)\sdo_test.bsc"
   copy ..\..\..\deploy\bin\*.dll Debug
	copy ..\..\..\deploy\bin\*.pdb Debug
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
!IF EXISTS("sdo_test.dep")
!INCLUDE "sdo_test.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_test.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_test - Win32 Release" || "$(CFG)" == "sdo_test - Win32 Debug"
SOURCE=..\..\..\runtime\core\test\main.cpp

!IF  "$(CFG)" == "sdo_test - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\runtime\core\test\SdoGenerate.cpp

!IF  "$(CFG)" == "sdo_test - Win32 Release"


"$(INTDIR)\SdoGenerate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"


"$(INTDIR)\SdoGenerate.obj"	"$(INTDIR)\SdoGenerate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\runtime\core\test\sdotest.cpp

!IF  "$(CFG)" == "sdo_test - Win32 Release"


"$(INTDIR)\sdotest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"


"$(INTDIR)\sdotest.obj"	"$(INTDIR)\sdotest.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\runtime\core\test\sdotest2.cpp

!IF  "$(CFG)" == "sdo_test - Win32 Release"


"$(INTDIR)\sdotest2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"


"$(INTDIR)\sdotest2.obj"	"$(INTDIR)\sdotest2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\runtime\core\test\utils.cpp

!IF  "$(CFG)" == "sdo_test - Win32 Release"


"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"


"$(INTDIR)\utils.obj"	"$(INTDIR)\utils.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

!IF  "$(CFG)" == "sdo_test - Win32 Release"

"sdo_runtime - Win32 Release" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" 
   cd "..\sdo_test"

"sdo_runtime - Win32 ReleaseCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" RECURSE=1 CLEAN 
   cd "..\sdo_test"

!ELSEIF  "$(CFG)" == "sdo_test - Win32 Debug"

"sdo_runtime - Win32 Debug" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" 
   cd "..\sdo_test"

"sdo_runtime - Win32 DebugCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\sdo_test"

!ENDIF 


!ENDIF 

