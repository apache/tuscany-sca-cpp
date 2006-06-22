# Microsoft Developer Studio Generated NMAKE File, Based on sdo_axiom.dsp
!IF "$(CFG)" == ""
CFG=sdo_axiom - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sdo_axiom - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sdo_axiom - Win32 Release" && "$(CFG)" != "sdo_axiom - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdo_axiom.mak" CFG="sdo_axiom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdo_axiom - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sdo_axiom - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sdo_axiom - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\sdo_axiom.dll"

!ELSE 

ALL : "sdo_runtime - Win32 Release" "$(OUTDIR)\sdo_axiom.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\sdo_axiom.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\sdo_axiom.dll"
	-@erase "$(OUTDIR)\sdo_axiom.exp"
	-@erase "$(OUTDIR)\sdo_axiom.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_AXIOM_EXPORTS" /Fp"$(INTDIR)\sdo_axiom.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_axiom.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\sdo_axiom.pdb" /machine:I386 /out:"$(OUTDIR)\sdo_axiom.dll" /implib:"$(OUTDIR)\sdo_axiom.lib" 
LINK32_OBJS= \
	"$(INTDIR)\sdo_axiom.obj" \
	"..\..\..\runtime\core\Release\tuscany_sdo.lib"

"$(OUTDIR)\sdo_axiom.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sdo_axiom - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\tuscany_sdo_axiom.dll"

!ELSE 

ALL : "sdo_runtime - Win32 Debug" "$(OUTDIR)\tuscany_sdo_axiom.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"sdo_runtime - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\sdo_axiom.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tuscany_sdo_axiom.dll"
	-@erase "$(OUTDIR)\tuscany_sdo_axiom.exp"
	-@erase "$(OUTDIR)\tuscany_sdo_axiom.ilk"
	-@erase "$(OUTDIR)\tuscany_sdo_axiom.lib"
	-@erase "$(OUTDIR)\tuscany_sdo_axiom.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\deploy\include" /I "$(AXIS2C_HOME)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDO_AXIOM_EXPORTS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sdo_axiom.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=axis2_parser.lib axis2_util.lib axiom.lib tuscany_sdo.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tuscany_sdo_axiom.pdb" /debug /machine:I386 /out:"$(OUTDIR)\tuscany_sdo_axiom.dll" /implib:"$(OUTDIR)\tuscany_sdo_axiom.lib" /pdbtype:sept /libpath:"..\..\..\deploy\lib" /libpath:"$(AXIS2C_HOME)\lib" 
LINK32_OBJS= \
	"$(INTDIR)\sdo_axiom.obj" \
	"..\..\..\runtime\core\Debug\tuscany_sdo.lib"

"$(OUTDIR)\tuscany_sdo_axiom.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "sdo_runtime - Win32 Debug" "$(OUTDIR)\tuscany_sdo_axiom.dll"
   copy       Debug\tuscany_sdo_axiom.dll       ..\..\..\deploy\bin
	copy       Debug\tuscany_sdo_axiom.pdb       ..\..\..\deploy\bin
	copy       Debug\tuscany_sdo_axiom.lib       ..\..\..\deploy\lib
	copy       ..\..\..\runtime\core\sdo_axiom\sdo_axiom.h       ..\..\..\deploy\include
	copy       ..\..\..\runtime\core\sdo_axiom\sdo_axiom_export.h       ..\..\..\deploy\include

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sdo_axiom.dep")
!INCLUDE "sdo_axiom.dep"
!ELSE 
!MESSAGE Warning: cannot find "sdo_axiom.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sdo_axiom - Win32 Release" || "$(CFG)" == "sdo_axiom - Win32 Debug"
SOURCE=..\..\..\runtime\core\sdo_axiom\sdo_axiom.cpp

"$(INTDIR)\sdo_axiom.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "sdo_axiom - Win32 Release"

"sdo_runtime - Win32 Release" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" 
   cd "..\sdo_axiom"

"sdo_runtime - Win32 ReleaseCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Release" RECURSE=1 CLEAN 
   cd "..\sdo_axiom"

!ELSEIF  "$(CFG)" == "sdo_axiom - Win32 Debug"

"sdo_runtime - Win32 Debug" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" 
   cd "..\sdo_axiom"

"sdo_runtime - Win32 DebugCLEAN" : 
   cd "..\sdo_runtime"
   $(MAKE) /$(MAKEFLAGS) /F .\sdo_runtime.mak CFG="sdo_runtime - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\sdo_axiom"

!ENDIF 


!ENDIF 

