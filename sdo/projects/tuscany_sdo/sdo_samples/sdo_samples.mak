# Microsoft Developer Studio Generated NMAKE File, Based on sdo_samples.dsp
!IF "$(CFG)" == ""
CFG=sdo_samples - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sdo_samples - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sdo_samples - Win32 Release" && "$(CFG)" != "sdo_samples - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdo_samples.mak" CFG="sdo_samples - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdo_samples - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "sdo_samples - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sdo_samples - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sdo_samples.exe"

!ELSE 

ALL : "sdo_runtime - Win32 Release" "$(OUTDIR)\sdo_samples.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ChangeSummarySave.obj"
	-@erase "$(INTDIR)\ObjectCreation.obj"
	-@erase "$(INTDIR)\Query.obj"
	-@erase "$(INTDIR)\samples.obj"
	-@erase "$(INTDIR)\Substitutes.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\XSDLoading.obj"
	-@erase "$(OUTDIR)\sdo_samples.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\..\..\deploy\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_samples.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\sdo_samples.pdb" /machine:I386 /out:"$(OUTDIR)\sdo_samples.exe" /libpath:"..\..\..\deploy\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangeSummarySave.obj" \
	"$(INTDIR)\ObjectCreation.obj" \
	"$(INTDIR)\Query.obj" \
	"$(INTDIR)\samples.obj" \
	"$(INTDIR)\Substitutes.obj" \
	"$(INTDIR)\XSDLoading.obj" \
	"..\sdo_runtime\Release\tuscany_sdo.lib"

"$(OUTDIR)\sdo_samples.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Release" "$(OUTDIR)\sdo_samples.exe"
   copy ..\..\..\runtime\core\samples\*.xsd ..\..\..\runtime\core\samples\Release
	copy ..\..\..\runtime\core\samples\*.xml ..\..\..\runtime\core\samples\Release
	copy ..\..\..\runtime\core\samples\*.wsdl ..\..\..\runtime\core\samples\Release
	copy ..\..\..\deploy\bin\*.dll ..\..\..\runtime\core\samples\Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "sdo_samples - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\samples.exe"

!ELSE 

ALL : "sdo_runtime - Win32 Debug" "$(OUTDIR)\samples.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\ChangeSummarySave.obj"
	-@erase "$(INTDIR)\ObjectCreation.obj"
	-@erase "$(INTDIR)\Query.obj"
	-@erase "$(INTDIR)\samples.obj"
	-@erase "$(INTDIR)\Substitutes.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XSDLoading.obj"
	-@erase "$(OUTDIR)\samples.exe"
	-@erase "$(OUTDIR)\samples.ilk"
	-@erase "$(OUTDIR)\samples.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\deploy\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_samples.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\samples.pdb" /debug /machine:I386 /out:"$(OUTDIR)\samples.exe" /pdbtype:sept /libpath:"..\..\..\deploy\lib" 
LINK32_OBJS= \
	"$(INTDIR)\ChangeSummarySave.obj" \
	"$(INTDIR)\ObjectCreation.obj" \
	"$(INTDIR)\Query.obj" \
	"$(INTDIR)\samples.obj" \
	"$(INTDIR)\Substitutes.obj" \
	"$(INTDIR)\XSDLoading.obj" \
	"..\sdo_runtime\Debug\tuscany_sdo.lib"

"$(OUTDIR)\samples.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Debug" "$(OUTDIR)\samples.exe"
   copy ..\..\..\runtime\core\samples\*.xsd ..\..\..\runtime\core\samples\Debug
	copy ..\..\..\runtime\core\samples\*.xml ..\..\..\runtime\core\samples\Debug
	copy ..\..\..\deploy\bin\*.dll ..\..\..\runtime\core\samples\Debug
	copy ..\..\..\deploy\bin\*.pdb ..\..\..\runtime\core\samples\Debug
	copy ..\..\..\runtime\core\samples\*.wsdl ..\..\..\runtime\core\samples\Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sdo_samples.dep")
!INCLUDE "sdo_samples.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_samples.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_samples - Win32 Release" || "$(CFG)" == "sdo_samples - Win32 Debug"
SOURCE=..\..\..\runtime\core\samples\ChangeSummarySave.cpp

"$(INTDIR)\ChangeSummarySave.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\samples\ObjectCreation.cpp

"$(INTDIR)\ObjectCreation.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\samples\Query.cpp

"$(INTDIR)\Query.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\samples\samples.cpp

"$(INTDIR)\samples.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\samples\Substitutes.cpp

"$(INTDIR)\Substitutes.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\runtime\core\samples\XSDLoading.cpp

"$(INTDIR)\XSDLoading.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "sdo_samples - Win32 Release"

"sdo_runtime - Win32 Release" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" 
   cd "..\sdo_samples"

"sdo_runtime - Win32 ReleaseCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" RECURSE=1 CLEAN 
   cd "..\sdo_samples"

!ELSEIF  "$(CFG)" == "sdo_samples - Win32 Debug"

"sdo_runtime - Win32 Debug" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" 
   cd "..\sdo_samples"

"sdo_runtime - Win32 DebugCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\sdo_samples"

!ENDIF 


!ENDIF 

