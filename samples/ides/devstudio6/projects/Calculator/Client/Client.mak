# Microsoft Developer Studio Generated NMAKE File, Based on Client.dsp
!IF "$(CFG)" == ""
CFG=Client - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Client - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Client - Win32 Release" && "$(CFG)" != "Client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Client - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Client.exe"

!ELSE 

ALL : "Calculator - Win32 Release" "$(OUTDIR)\Client.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Calculator - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Calc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Client.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\Client.pdb" /machine:I386 /out:"$(OUTDIR)\Client.exe" /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\Calc.obj" \
	"..\Calculator\Release\Calculator.lib"

"$(OUTDIR)\Client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

$(DS_POSTBUILD_DEP) : "Calculator - Win32 Release" "$(OUTDIR)\Client.exe"
   ..\deploy.cmd Release
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Calc.exe"

!ELSE 

ALL : "Calculator - Win32 Debug" "$(OUTDIR)\Calc.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Calculator - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Calc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Calc.exe"
	-@erase "$(OUTDIR)\Calc.ilk"
	-@erase "$(OUTDIR)\Calc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(TUSCANY_SDOCPP)/include" /I "$(TUSCANY_SCACPP)/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib tuscany_sdo.lib tuscany_sca.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\Calc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Calc.exe" /pdbtype:sept /libpath:"$(TUSCANY_SDOCPP)/lib" /libpath:"$(TUSCANY_SCACPP)/lib" 
LINK32_OBJS= \
	"$(INTDIR)\Calc.obj" \
	"..\Calculator\Debug\Calculator.lib"

"$(OUTDIR)\Calc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

$(DS_POSTBUILD_DEP) : "Calculator - Win32 Debug" "$(OUTDIR)\Calc.exe"
   ..\deploy.cmd Debug
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Client.dep")
!INCLUDE "Client.dep"
!ELSE 
!MESSAGE Warning: cannot find "Client.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Client - Win32 Release" || "$(CFG)" == "Client - Win32 Debug"
SOURCE=..\..\..\..\..\Calculator\Client\Calc.cpp

"$(INTDIR)\Calc.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "Client - Win32 Release"

"Calculator - Win32 Release" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Release" 
   cd "..\Client"

"Calculator - Win32 ReleaseCLEAN" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Client"

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

"Calculator - Win32 Debug" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Debug" 
   cd "..\Client"

"Calculator - Win32 DebugCLEAN" : 
   cd "..\Calculator"
   $(MAKE) /$(MAKEFLAGS) /F .\Calculator.mak CFG="Calculator - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Client"

!ENDIF 


!ENDIF 

